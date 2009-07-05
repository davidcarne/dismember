#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QAbstractItemModel>
#include <QLineEdit>
#include "codeview.h"
#include "document.h"
#include "documentproxymodel.h"

CodeView::CodeView(QWidget *parent)
 : QTableView(parent)
{
	resizeColumnsToContents();
	resizeRowsToContents();
	setShowGrid(false);
	verticalHeader()->hide();
	//horizontalHeader()->hide();
	horizontalHeader()->setStretchLastSection(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CodeView::keyPressEvent(QKeyEvent *event)
{
	int row = selectionModel()->currentIndex().row();
	DocumentProxyModel *dpm = (DocumentProxyModel *)model();
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		if (!m_jumpStack.empty()) {
			row = m_jumpStack.pop();
			QModelIndex idx = dpm->index(row, 0, QModelIndex());
			scrollTo(idx);
			setCurrentIndex(idx);
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_G:
		if (row != -1 && (row = dpm->getJumpLine(row)) != -1) {
			QModelIndex idx = dpm->index(row, 0, QModelIndex());
			m_jumpStack.push(selectionModel()->currentIndex().row());
			scrollTo(idx);
			setCurrentIndex(idx);
		}
		break;
	case Qt::Key_C:
	case Qt::Key_I:
		if (row != -1)
			dpm->analyze(row);
		break;
	case Qt::Key_D:
		// define as data type
		break;
	case Qt::Key_U:
		if (row != -1)
			dpm->undefine(row);
		break;
	case Qt::Key_S:
		if (row != -1) {
			bool ok;
			QString ntext = QInputDialog::getText(this,
					tr("Symbol"),
					tr("Symbol name"),
					QLineEdit::Normal,
					dpm->getSymbol(row), &ok);
			if (ok && !ntext.isEmpty())
				dpm->setSymbol(row, ntext);
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

void CodeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	int row = indexAt(event->pos()).row();
	DocumentProxyModel *dpm = (DocumentProxyModel *)model();
	if (row != -1 && (row = dpm->getJumpLine(row)) != -1) {
		QModelIndex idx = dpm->index(row, 0, QModelIndex());
		m_jumpStack.push(selectionModel()->currentIndex().row());
		scrollTo(idx);
		setCurrentIndex(idx);
	}
}

void CodeView::contextMenuEvent(QContextMenuEvent *event)
{
	int row = indexAt(event->pos()).row();
	if (row == -1) return;
	DocumentProxyModel *dpm = (DocumentProxyModel *)model();
	QList<QAction *> actions;
	actions.push_back(new QAction(tr("Set Symbol"), this));
	if (dpm->isDefined(row))
		actions.push_back(new QAction(tr("Undefine"), this));
	else
		actions.push_back(new QAction(tr("Analyze Code"), this));
	if (dpm->getJumpLine(row) != -1)
		actions.push_back(new QAction(tr("Jump to branch"), this));

	QAction *action = QMenu::exec(actions, event->globalPos(),
			(QAction *)0, this);
	if (action) {
		// todo: using names here for compare sucks
		if (action->text() == tr("Analyze Code")) {
			dpm->analyze(row);
		} else if (action->text() == tr("Undefine")) {
			dpm->undefine(row);
		} else if (action->text() == tr("Set Symbol")) {
			bool ok;
			QString ntext = QInputDialog::getText(this,
					tr("Symbol"),
					tr("Symbol name"),
					QLineEdit::Normal,
					dpm->getSymbol(row), &ok);
			if (ok && !ntext.isEmpty())
				dpm->setSymbol(row, ntext);
		} else if (action->text() == tr("Jump to branch")) {
			int nrow = dpm->getJumpLine(row);
			QModelIndex idx = dpm->index(nrow, 0, QModelIndex());
			m_jumpStack.push(row);
			scrollTo(idx);
			setCurrentIndex(idx);
		}
	}
	while (!actions.isEmpty())
		delete actions.takeFirst();
}
