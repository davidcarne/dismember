#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QFontMetrics>
#include <QInputDialog>
#include <QAbstractItemModel>
#include <QLineEdit>

#include "document.h"
#include "memlocdata.h"
#include "codeview.h"
#include "codemodel.h"
#include "multilinedialog.h"
#include "program_flow_analysis.h"
#include "symbol_analysis.h"

class QTImmutableCodeController : public QTCodeController
{
 public:
	QTImmutableCodeController(QTRuntimeModel &model)
	 : QTCodeController(model)
	{ }

	virtual void update() { }
	virtual void analyze(address_t addr)  { }
	virtual void undefine(address_t addr)  { }
	virtual void setComment(address_t addr, const QString &cmt)  { }
	virtual void setSymbol(address_t addr, const QString &sym)  { }
};

class QTMutableCodeController : public QTCodeController
{
 public:
	QTMutableCodeController(QTRuntimeModel &model)
	 : QTCodeController(model), m_model(model)
	{ }

	virtual void update()
	{
		m_model.getProxy().update();
	}

	virtual void analyze(address_t addr)
	{
		ProgramFlowAnalysis::submitAnalysisJob(&m_model.getRuntime(),
				m_model.getTrace().getCodeDataType(), addr);
		SymbolAnalysis::submitAnalysisJob(&m_model.getRuntime());
	}

	virtual void undefine(address_t addr)
	{
		ProgramFlowAnalysis::submitUndefineJob(&m_model.getRuntime(),
				addr);
	}

	virtual void setComment(address_t addr, const QString &cmt)
	{
		m_model.getTrace().create_comment(cmt.toStdString(), addr);
	}

	virtual void setSymbol(address_t addr, const QString &sym)
	{
		m_model.getTrace().create_sym(sym.toStdString(), addr);
	}

 private:
	QTRuntimeModel &m_model;
};

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
	if (m_controller)
		delete m_controller;
	m_model = new QTCodeModel(model);
	m_controller = new QTMutableCodeController(*model);
	setModel(m_model);
}

void QTCodeView::runtimeUpdated(QTRuntimeEvent *evt)
{
	int row = selectionModel()->currentIndex().row();
	address_t addr;
	if (row != -1)
		addr = m_runtime->getProxy().getLineAddr(row);

	switch (evt->type()) {
	case QTRuntimeEvent::RuntimeJump:
		if (row != -1 && evt->flags() & QTRuntimeEvent::Select)
			m_jumpStack.push(addr);
		scrollTo(evt->start());
		if (row != -1 && evt->flags() & QTRuntimeEvent::Select)
			setCurrentIndex(evt->start());
		break;
	case QTRuntimeEvent::RuntimeUpdate:
		// we don't currently know how to update a range, flush instead
	case QTRuntimeEvent::RuntimeFlush:
		m_controller->update();
		m_model->flush();
		if (row != -1) {
			scrollTo(addr);
			setCurrentIndex(addr);
		}
		break;
	default: break;
	}
}

void QTCodeView::scrollTo(address_t addr)
{
	try {
		int row = m_runtime->getProxy().getLineAtAddr(addr);
		QModelIndex idx = m_model->index(row, 0, QModelIndex());
		QTableView::scrollTo(idx);
	} catch (std::out_of_range &) { }
}

void QTCodeView::setCurrentIndex(address_t addr)
{
	try {
		int row = m_runtime->getProxy().getLineAtAddr(addr);
		QModelIndex idx = m_model->index(row, 0, QModelIndex());
		QTableView::setCurrentIndex(idx);
	} catch (std::out_of_range &) { }
}

void QTCodeView::keyPressEvent(QKeyEvent *event)
{
	int row = selectionModel()->currentIndex().row();

	if (event->modifiers() != Qt::NoModifier)
		QTableView::keyPressEvent(event);

	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		if (!m_jumpStack.empty()) {
			address_t addr = m_jumpStack.pop();
			scrollTo(addr);
			setCurrentIndex(addr);
		}
		return;
	default: break;
	}

	if (row == -1) return;

	address_t addr = m_runtime->getProxy().getLineAddr(row);
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_G:
		jump(addr);
		break;
	case Qt::Key_C:
	case Qt::Key_I:
		m_controller->analyze(addr);
		break;
	case Qt::Key_D:
		// define as data type
		break;
	case Qt::Key_U:
		m_controller->undefine(addr);
		break;
	case Qt::Key_S:
		setSymbol(addr);
		// define symbol
		break;
	case Qt::Key_Semicolon:
		setComment(addr);
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
	QModelIndex idx = indexAt(event->pos());
	int row = idx.row();
	if (row == -1) return;
	address_t addr = m_runtime->getProxy().getLineAddr(row);
	switch (idx.column()) {
	case 0:
		break;
	case 1:
		setSymbol(addr);
		break;
	case 2:
		jump(addr);
		break;
	case 3:
		// todo: jump to xref if 1, else popup xref menu
		break;
	case 4:
		setComment(addr);
		break;
	}
}

void QTCodeView::setComment(address_t addr)
{
	bool ok;
	const Comment *com = m_runtime->getTrace().lookup_comment(addr);
	QString text = com ? QString(com->get_comment().c_str()) : QString();

	QString ntext = QTMultiLineDialog::getText(this,
			tr("Comment"), tr("Comment:"), text, &ok);
	if (ok && !ntext.isEmpty()) {
		m_controller->setComment(addr, ntext);
		setCurrentIndex(addr);
		m_runtime->postUpdate();
	}
}

void QTCodeView::setSymbol(address_t addr)
{
	bool ok;
	const Symbol *sym = m_runtime->getTrace().lookup_symbol(addr);
	QString text = sym ? QString(sym->get_name().c_str()) : QString();

	QString ntext = QInputDialog::getText(this,
			tr("Symbol"), tr("Symbol:"),
			QLineEdit::Normal, text, &ok);
	if (ok && !ntext.isEmpty()) {
		m_controller->setSymbol(addr, ntext);
		setCurrentIndex(addr);
		m_runtime->postUpdate();
	}
}

bool QTCodeView::hasJump(address_t addr)
{
	MemlocData * i = m_runtime->getTrace().lookup_memloc(addr);
	return (i && i->has_xrefs_from());
}

bool QTCodeView::isDefined(address_t addr)
{
	return m_runtime->getTrace().lookup_memloc(addr) != 0;
}

void QTCodeView::jump(address_t addr)
{
	MemlocData * i = m_runtime->getTrace().lookup_memloc(addr);
	if (i && i->has_xrefs_from()) {
		Xref * x = (*(i->begin_xref_from())).second;
		address_t naddr = x->get_dst_addr();
		m_jumpStack.push(addr);
		scrollTo(naddr);
		setCurrentIndex(naddr);
	}
}

void QTCodeView::contextMenuEvent(QContextMenuEvent *event)
{
	bool ok;
	int row = indexAt(event->pos()).row();
	if (row == -1) return;
	address_t addr = m_runtime->getProxy().getLineAddr(row);
	QList<QAction *> actions;
	actions.push_back(new QAction(tr("Set Symbol"), this));
	actions.push_back(new QAction(tr("Set Comment"), this));
	if (isDefined(addr))
		actions.push_back(new QAction(tr("Undefine"), this));
	else
		actions.push_back(new QAction(tr("Analyze Code"), this));
	if (hasJump(addr))
		actions.push_back(new QAction(tr("Jump to branch"), this));

	QAction *action = QMenu::exec(actions, event->globalPos(),
			(QAction *)0, this);
	if (action) {
		// todo: using names here for compare sucks
		if (action->text() == tr("Analyze Code")) {
			m_controller->analyze(addr);
		} else if (action->text() == tr("Undefine")) {
			m_controller->undefine(addr);
		} else if (action->text() == tr("Set Symbol")) {
			setSymbol(addr);
		} else if (action->text() == tr("Set Comment")) {
			setComment(addr);
		} else if (action->text() == tr("Jump to branch")) {
			jump(addr);
		}
	}
	while (!actions.isEmpty())
		delete actions.takeFirst();
}
