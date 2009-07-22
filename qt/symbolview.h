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

#ifndef _SYMBOLVIEW_H_
#define _SYMBOLVIEW_H_
#include <QListView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QStack>

#include "runtimemodel.h"
#include "symbolmodel.h"

class QTSymbolView : public QListView, public QTRuntimeModelListener
{
 public:
	QTSymbolView(QWidget *parent = NULL);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

 private:
	QTSymbolModel *m_model;
	QTRuntimeModel *m_runtime;
};

#endif
