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

#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

#include <vector>
#include "runtimemodel.h"

class Document;

class QTDataView : public QWidget, public QTRuntimeModelListener
{
 public:
	QTDataView(QWidget *parent);

	void setRuntimeModel(QTRuntimeModel *rt);
	void runtimeUpdated(QTRuntimeEvent *m);

 protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

	QTRuntimeModel *m_model;
	bool m_mouseActive;
	bool m_mouseInside;
	std::vector<address_t> m_paddrMap;
};

#endif
