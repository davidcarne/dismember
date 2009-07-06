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
