#ifndef _CODEVIEW_H_
#define _CODEVIEW_H_
#include <QTableView>
#include <QKeyEvent>
#include <QContextMenuEvent>

class Document;

class CodeView : public QTableView
{
 public:
	CodeView(QWidget *parent = NULL);

	void contextMenuEvent(QContextMenuEvent *event);
	void keyPressEvent(QKeyEvent *event);
};

#endif
