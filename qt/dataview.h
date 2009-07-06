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
