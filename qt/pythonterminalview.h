#ifndef _PYTHONTERMINALVIEW_H_
#define _PYTHONTERMINALVIEW_H_

#include <QWidget>
#include <QString>
#include <QList>
#include <QTextCursor>
#include <QPlainTextEdit>

#include "runtimemodel.h"
#include "localpythoninterpreter.h"

class QTPythonTerminalView
 : public QPlainTextEdit, public QTRuntimeModelListener,
 		public LPIOutputStreamAcceptor
{
 public:
	QTPythonTerminalView(QWidget *parent = 0);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);
	void keyPressEvent(QKeyEvent *event);

	void write(const std::string &data, int stream);
	void refresh(bool);

 private:
	QTRuntimeModel *m_runtime;
	QString m_command;
	QTextCursor m_cursor;
	int m_offset;
	int m_history;
	QList<QString> m_commandHistory;
};

#endif
