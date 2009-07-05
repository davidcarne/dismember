#ifndef _CODEVIEW_H_
#define _CODEVIEW_H_
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QStack>

class Document;

class CodeView : public QTableView
{
 public:
	CodeView(QWidget *parent = NULL);

	void contextMenuEvent(QContextMenuEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

 private:
	QStack<int> m_jumpStack;
};

#endif
