#ifndef _CODEVIEW_H_
#define _CODEVIEW_H_
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QStack>

#include "runtimemodel.h"
#include "codemodel.h"

class QTCodeView : public QTableView, public QTRuntimeModelListener
{
 public:
	QTCodeView(QWidget *parent = NULL);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void contextMenuEvent(QContextMenuEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

 private:
	QTCodeModel *m_model;
	QStack<int> m_jumpStack;
};

#endif
