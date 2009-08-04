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
		Extend,
		Execute
	};

 protected:
	void insertFromMimeData(const QMimeData *source);
	void contextMenuEvent(QContextMenuEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void keyPressEvent(QKeyEvent *event);

 private:
	void exec();
	void extend();
	void refresh(bool);
	void keyCommand(KeyCommand kc);

	QTRuntimeModel *m_runtime;
	QString m_command;
	QString m_fullCommand;
	QString m_prompt;
	QTextCursor m_cursor;
	int m_offset;
	int m_history;
	uint8_t m_modifiers;
	QList<QString> m_commandHistory;
};

#endif
