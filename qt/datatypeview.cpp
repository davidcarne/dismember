/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QApplication>
#include "document.h"
#include "datatypeview.h"

QTDataTypeView::QTDataTypeView(QWidget *parent)
 : QListWidget(parent), m_runtime(NULL), m_codeview(NULL)
{ }

void QTDataTypeView::setRuntimeModel(QTRuntimeModel *model)
{
	m_runtime = model;
	DataTypeReg::datatypereg_ci di = model->getI_ProjectModel().getDataTypeBegin();
	DataTypeReg::datatypereg_ci end = model->getI_ProjectModel().getDataTypeEnd();
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
	sp_DataType d = m_runtime->getI_ProjectModel().lookupDataType(str.toStdString());
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
