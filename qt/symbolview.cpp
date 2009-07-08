#include <QApplication>
#include "document.h"
#include "symbolview.h"

QTSymbolView::QTSymbolView(QWidget *parent)
 : QListView(parent), m_model(NULL), m_runtime(NULL)
{ }

void QTSymbolView::setRuntimeModel(QTRuntimeModel *model)
{
	m_runtime = model;
	if (m_model)
		delete m_model;
	m_model = new QTSymbolModel(model);
	setModel(m_model);
}

void QTSymbolView::runtimeUpdated(QTRuntimeEvent *evt)
{
	switch (evt->type()) {
	case QTRuntimeEvent::RuntimeJump:
		break;
	case QTRuntimeEvent::RuntimeUpdate:
	case QTRuntimeEvent::RuntimeFlush:
		m_model->flush();
		break;
	default: break;
	}
}

void QTSymbolView::keyPressEvent(QKeyEvent *event)
{
	int row = selectionModel()->currentIndex().row();
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_G:
		m_runtime->postJump(m_model->getAddress(row),
				QTRuntimeEvent::Select);
		break;
	default:
		QListView::keyPressEvent(event);
		break;
	}
}

void QTSymbolView::mouseDoubleClickEvent(QMouseEvent *event)
{
	int row = indexAt(event->pos()).row();
	m_runtime->postJump(m_model->getAddress(row),
			QTRuntimeEvent::Select);
}
