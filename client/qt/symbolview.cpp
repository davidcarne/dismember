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
#include "workspace.h"
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
