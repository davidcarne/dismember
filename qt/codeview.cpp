#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QFontMetrics>
#include <QApplication>
#include <QInputDialog>
#include <QAbstractItemModel>
#include <QLineEdit>
#include "document.h"
#include "codeview.h"
#include "codemodel.h"

QTCodeView::QTCodeView(QWidget *parent)
 : QTableView(parent), m_model(NULL), m_runtime(NULL)
{
	setShowGrid(false);
	verticalHeader()->hide();
	verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 1);
	setColumnWidth(0, fontMetrics().maxWidth() * 9);
	setColumnWidth(1, fontMetrics().maxWidth() * 24);
	setColumnWidth(2, fontMetrics().maxWidth() * 40);
	setColumnWidth(3, fontMetrics().maxWidth() * 12);
	horizontalHeader()->setStretchLastSection(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
}

void QTCodeView::setRuntimeModel(QTRuntimeModel *model)
{
	m_runtime = model;
	if (m_model)
		delete m_model;
	m_model = new QTCodeModel(model);
	setModel(m_model);
}

void QTCodeView::runtimeUpdated(QTRuntimeEvent *evt)
{
	int row = selectionModel()->currentIndex().row();
	switch (evt->type()) {
	case QTRuntimeEvent::RuntimeJump: {
		if (evt->flags() & QTRuntimeEvent::Select)
			m_jumpStack.push(row);
		int row = m_runtime->getProxy().getLineAtAddr(evt->start());
		scrollTo(row);
		if (evt->flags() & QTRuntimeEvent::Select)
			setCurrentIndex(row);
		} break;
	case QTRuntimeEvent::RuntimeUpdate:
		// we don't currently know how to update a range, flush instead
	case QTRuntimeEvent::RuntimeFlush: {
		GuiProxy &prox = m_runtime->getProxy();
		int row = selectionModel()->currentIndex().row();
		if (row != -1) {
			address_t addr = prox.getLineAddr(row);
			m_model->flush();
			row = prox.getLineAtAddr(addr);
			scrollTo(row);
			setCurrentIndex(row);
		} else
			m_model->flush();

		} break;
	default: break;
	}
}

void QTCodeView::scrollTo(int row)
{
	QModelIndex idx = m_model->index(row, 0, QModelIndex());
	QTableView::scrollTo(idx);
}

void QTCodeView::setCurrentIndex(int row)
{
	QModelIndex idx = m_model->index(row, 0, QModelIndex());
	QTableView::setCurrentIndex(idx);
}

void QTCodeView::keyPressEvent(QKeyEvent *event)
{
	int row = selectionModel()->currentIndex().row();
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		if (!m_jumpStack.empty()) {
			row = m_jumpStack.pop();
			scrollTo(row);
			setCurrentIndex(row);
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_G:
		if (row != -1 && (row = m_model->getJumpLine(row)) != -1) {
			m_jumpStack.push(selectionModel()->currentIndex().row());
			scrollTo(row);
			setCurrentIndex(row);
		}
		break;
	case Qt::Key_C:
	case Qt::Key_I:
		if (row != -1)
			m_model->analyze(row);
		break;
	case Qt::Key_D:
		// define as data type
		break;
	case Qt::Key_U:
		if (row != -1)
			m_model->undefine(row);
		break;
	case Qt::Key_S:
		if (row != -1)
			setSymbol(row);
		// define symbol
		break;
	case Qt::Key_Semicolon:
		// define comment
		break;
	case Qt::Key_Plus:
		// expand
		break;
	case Qt::Key_Minus:
		// collapse
		break;
	default:
		// todo: filter out everything that'll cause a search.
		QTableView::keyPressEvent(event);
		break;
	}
}

void QTCodeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	int row = indexAt(event->pos()).row();
	if (row != -1 && (row = m_model->getJumpLine(row)) != -1) {
		m_jumpStack.push(selectionModel()->currentIndex().row());
		scrollTo(row);
		setCurrentIndex(row);
	}
}

void QTCodeView::setSymbol(int row)
{
	bool ok;
	QString ntext = QInputDialog::getText(this,
			tr("Symbol"),
			tr("Symbol name"),
			QLineEdit::Normal,
			m_model->getSymbol(row), &ok);
	if (ok && !ntext.isEmpty()) {
		m_model->setSymbol(row, ntext);
		setCurrentIndex(row);
		m_runtime->postUpdate();
	}
}

void QTCodeView::contextMenuEvent(QContextMenuEvent *event)
{
	int row = indexAt(event->pos()).row();
	if (row == -1) return;
	QList<QAction *> actions;
	actions.push_back(new QAction(tr("Set Symbol"), this));
	if (m_model->isDefined(row))
		actions.push_back(new QAction(tr("Undefine"), this));
	else
		actions.push_back(new QAction(tr("Analyze Code"), this));
	if (m_model->getJumpLine(row) != -1)
		actions.push_back(new QAction(tr("Jump to branch"), this));

	QAction *action = QMenu::exec(actions, event->globalPos(),
			(QAction *)0, this);
	if (action) {
		// todo: using names here for compare sucks
		if (action->text() == tr("Analyze Code")) {
			m_model->analyze(row);
		} else if (action->text() == tr("Undefine")) {
			m_model->undefine(row);
		} else if (action->text() == tr("Set Symbol")) {
			setSymbol(row);
		} else if (action->text() == tr("Jump to branch")) {
			int nrow = m_model->getJumpLine(row);
			m_jumpStack.push(row);
			scrollTo(row);
			setCurrentIndex(row);
		}
	}
	while (!actions.isEmpty())
		delete actions.takeFirst();
}
