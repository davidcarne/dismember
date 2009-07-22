#include <QApplication>
#include "document.h"
#include "datatypeview.h"

QTDataTypeView::QTDataTypeView(QWidget *parent)
 : QListWidget(parent), m_runtime(NULL), m_codeview(NULL)
{ }

void QTDataTypeView::setRuntimeModel(QTRuntimeModel *model)
{
	m_runtime = model;
	DataTypeReg::datatypereg_ci di = model->getProjectModel().getDataTypeBegin();
	DataTypeReg::datatypereg_ci end = model->getProjectModel().getDataTypeEnd();
	clear();
	for (; di != end; ++di) {
		sp_DataType dt = (*di).second;
		std::string name = dt->getName();
		addItem(QString(name.c_str()));
	}
}

void QTDataTypeView::runtimeUpdated(QTRuntimeEvent *evt)
{
	switch (evt->type()) {
	case QTRuntimeEvent::RuntimeJump:
		break;
	case QTRuntimeEvent::RuntimeUpdate:
	case QTRuntimeEvent::RuntimeFlush:
		break;
	default: break;
	}
}

void QTDataTypeView::keyPressEvent(QKeyEvent *event)
{
	QListWidgetItem *itm = currentItem();
	if (!itm) return;
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
		createDataType(itm->text());
		break;
	default:
		QListWidget::keyPressEvent(event);
		break;
	}
}

void QTDataTypeView::setCodeview(QTCodeView *cv)
{
	m_codeview = cv;
}

void QTDataTypeView::createDataType(const QString &str)
{
	sp_DataType d = m_runtime->getProjectModel().lookupDataType(str.toStdString());
	address_t addr;
	if (m_codeview->getSelectionAddress(&addr))
		m_codeview->getController()->setDataType(addr, d.get());
	m_runtime->postUpdate();
}

void QTDataTypeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QListWidgetItem *itm = itemAt(event->pos());
	if (itm)
		createDataType(itm->text());
}
