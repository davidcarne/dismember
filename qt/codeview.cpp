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
 : QTableView(parent), m_model(NULL)
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
	if (m_model)
		delete m_model;
	m_model = new QTCodeModel(model);
	setModel(m_model);
}

void QTCodeView::runtimeUpdated(QTRuntimeEvent *evt)
{
	switch (evt->type()) {
	case QTRuntimeEvent::RuntimeJump: {
		int row = evt->model()->getProxy().getLineAtAddr(evt->start());
		QModelIndex idx = m_model->index(row, 0, QModelIndex());
		scrollTo(idx);
		} break;
	case QTRuntimeEvent::RuntimeUpdate:
		// we don't currently know how to update a range, flush instead
	case QTRuntimeEvent::RuntimeFlush:
		m_model->flush();
		break;
	default: break;
	}
}

void QTCodeView::keyPressEvent(QKeyEvent *event)
{
	int row = selectionModel()->currentIndex().row();
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		if (!m_jumpStack.empty()) {
			row = m_jumpStack.pop();
			QModelIndex idx = m_model->index(row, 0, QModelIndex());
			scrollTo(idx);
			setCurrentIndex(idx);
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_G:
		if (row != -1 && (row = m_model->getJumpLine(row)) != -1) {
			QModelIndex idx = m_model->index(row, 0, QModelIndex());
			m_jumpStack.push(selectionModel()->currentIndex().row());
			scrollTo(idx);
			setCurrentIndex(idx);
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
		if (row != -1) {
			bool ok;
			QString ntext = QInputDialog::getText(this,
					tr("Symbol"),
					tr("Symbol name"),
					QLineEdit::Normal,
					m_model->getSymbol(row), &ok);
			if (ok && !ntext.isEmpty())
				m_model->setSymbol(row, ntext);
		}
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
		QModelIndex idx = m_model->index(row, 0, QModelIndex());
		m_jumpStack.push(selectionModel()->currentIndex().row());
		scrollTo(idx);
		setCurrentIndex(idx);
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
			bool ok;
			QString ntext = QInputDialog::getText(this,
					tr("Symbol"),
					tr("Symbol name"),
					QLineEdit::Normal,
					m_model->getSymbol(row), &ok);
			if (ok && !ntext.isEmpty())
				m_model->setSymbol(row, ntext);
		} else if (action->text() == tr("Jump to branch")) {
			int nrow = m_model->getJumpLine(row);
			QModelIndex idx = m_model->index(nrow, 0, QModelIndex());
			m_jumpStack.push(row);
			scrollTo(idx);
			setCurrentIndex(idx);
		}
	}
	while (!actions.isEmpty())
		delete actions.takeFirst();
}
