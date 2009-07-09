#ifndef _PYTHONTERMINALVIEW_H_
#define _PYTHONTERMINALVIEW_H_

#include <QWidget>
#include <QString>
#include <QList>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QContextMenuEvent>

#include "runtimemodel.h"
#include "localpythoninterpreter.h"

class QTPythonTerminalView
 : public QPlainTextEdit, public QTRuntimeModelListener,
 		public LPIOutputStreamAcceptor
{
	Q_OBJECT
 public:
	QTPythonTerminalView(QWidget *parent = 0);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void write(const std::string &data, int stream);

	enum KeyCommand {
		BeginningOfLine,
		EndOfLine,
		ForwardChar,
		BackwardChar,
		ClearScreen,
		PreviousHistory,
		NextHistory,
		DeleteChar,
		BackwardDeleteChar,
		KillLine,
		Execute
	};

 protected:
	void insertFromMimeData(const QMimeData *source);
	void contextMenuEvent(QContextMenuEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void keyPressEvent(QKeyEvent *event);

 private:
	void exec(const QString &cmd);
	void refresh(bool);
	void keyCommand(KeyCommand kc);

	QTRuntimeModel *m_runtime;
	QString m_command;
	QTextCursor m_cursor;
	int m_offset;
	int m_history;
	uint8_t m_modifiers;
	QList<QString> m_commandHistory;
};

#endif
