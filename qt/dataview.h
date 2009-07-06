#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

#include <vector>

class Document;

class DataView : public QWidget
{
 public:
	DataView(QWidget *parent);

	void setDocument(Document *doc);
	void flush();

 protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

	Document *m_doc;
	bool m_mouseActive;
	bool m_mouseInside;
	std::vector<address_t> m_paddrMap;
};

#endif
