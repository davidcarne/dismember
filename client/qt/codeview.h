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

#ifndef _CODEVIEW_H_
#define _CODEVIEW_H_
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QShowEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QStack>
#include <QList>

#include "runtimemodel.h"
#include "codemodel.h"

// Apparently event->modifiers() doesn't work on linux
// But the workarond hack doesn't work on OSX
// #ifdef fun. c'est la vie.
#ifdef __APPLE__
#define USE_EVENT_MODIFIERS
#else
#endif

class QTCodeController
{
 public:
	QTCodeController(QTRuntimeModel &model) {};

	virtual void update() = 0;
	virtual void analyze(address_t addr) = 0;
	virtual void undefine(address_t addr) = 0;
	virtual void setComment(address_t addr, const QString &cmt) = 0;
	virtual void setSymbol(address_t addr, const QString &sym) = 0;
	virtual void setDataType(address_t addr, DataType *dt) = 0;
	virtual ~QTCodeController() = 0;
};

class QTCodeView : public QTableView, public QTRuntimeModelListener
{
 public:
	QTCodeView(QWidget *parent = NULL);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void showEvent(QShowEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void keyboardSearch(const QString &search);

	bool getSelectionAddress(address_t *addr);
	QTCodeController *getController();

 private:
 	void setSymbol(address_t addr);
	void setComment(address_t addr);
	void scrollTo(address_t addr);
	void setCurrentIndex(address_t addr);
	bool hasJump(address_t addr);
	bool isDefined(address_t addr);
	void jump(address_t addr);
	QList<QAction *> *createXrefMenu(address_t addr);

	QTCodeModel *m_model;
	QTCodeController *m_controller;
	QTRuntimeModel *m_runtime;
	
#ifndef USE_EVENT_MODIFIERS
	u8 m_modifiers;
#endif // USE_EVENT_MODIFIERS
	QStack<address_t> m_jumpStack;
};

#endif
