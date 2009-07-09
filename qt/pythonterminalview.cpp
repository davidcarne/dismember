#include <QApplication>
#include <QClipboard>
#include <QFontMetrics>
#include <QMenu>
#include "pythonterminalview.h"

QTPythonTerminalView::QTPythonTerminalView(QWidget *parent)
 : QPlainTextEdit(parent), m_offset(0), m_history(-1), m_modifiers(0)
{
	QFont font = QFont("fixed", 5);
	setFont(font);
	setCursorWidth(fontMetrics().width(" "));
	setTabStopWidth(fontMetrics().maxWidth()*4);
	setUndoRedoEnabled(false);
	setTextInteractionFlags(Qt::TextSelectableByMouse);
	setReadOnly(false);
}

void QTPythonTerminalView::setRuntimeModel(QTRuntimeModel *model)
{
	m_runtime = model;

	LocalPythonInterpreter *lpy = m_runtime->getRuntime().getPythonInterpreter();
	clear();
	lpy->getStderrRedirector()->setOutputAcceptor(this);
	lpy->getStdoutRedirector()->setOutputAcceptor(this);
	lpy->exec("import sys\nprint \"Python %s\" % sys.version");
	refresh(true);
}

void QTPythonTerminalView::runtimeUpdated(QTRuntimeEvent *m)
{ }

void QTPythonTerminalView::write(const std::string &data, int stream)
{
	textCursor().insertText(QString(data.c_str()));
}

// paste et al call this.
void QTPythonTerminalView::insertFromMimeData(const QMimeData *source)
{
	if (!source->hasText())
		return;

	QString str = source->text();
	if (str.contains("\n")) {
		QStringList lst = str.split("\n");
		QString remainder = "";
		if (m_offset != 0) {
			remainder = m_command.right(-m_offset);
			m_command.chop(-m_offset);
		}
		while (!lst.isEmpty()) {
			QString ss = lst.takeFirst();
			m_command += ss;
			exec(m_command);
		}
		m_command = remainder;
		refresh(false);
	} else {
		m_command.insert(m_command.length() + m_offset, str);
		refresh(false);
	}
}

void QTPythonTerminalView::refresh(bool clear)
{
	if (clear) {
		textCursor().insertText(QString(">>> "));
		m_cursor = textCursor();
	}
	while (!m_cursor.atEnd())
		m_cursor.deleteChar();
	int pos = m_cursor.position();
	m_cursor.insertText(m_command);
	m_cursor.setPosition(m_cursor.position() + m_offset);
	setTextCursor(m_cursor);
	m_cursor.setPosition(pos);
}

void QTPythonTerminalView::exec(const QString &cmd)
{
	LocalPythonInterpreter *lpy = m_runtime->getRuntime().getPythonInterpreter();
	try {
		QByteArray ba = cmd.toLocal8Bit();
		write("\n", 1);
		m_commandHistory.prepend(QString(cmd));
		boost::python::object res = lpy->execsingle(ba.constData());
	} catch (boost::python::error_already_set &) {
		PyErr_Print();
	}

	m_offset = 0;
	m_command = "";
	m_history = -1;
	refresh(true);
}

// we overload this to remove cut and delete from the context menu
void QTPythonTerminalView::contextMenuEvent(QContextMenuEvent *ev)
{
	QMenu *menu = createStandardContextMenu();
	QList<QAction *> actions = menu->actions();
	for (int i = 0; i < actions.count() ; ++i) {
		QString str = actions.at(i)->text();
		str.remove(QChar('&'));
		if (str.contains("Cut") || str.contains("Delete"))
			menu->removeAction(actions.at(i));
	}
	menu->exec(ev->globalPos());
	delete menu;	
}

void QTPythonTerminalView::keyCommand(QTPythonTerminalView::KeyCommand kc)
{
	switch (kc) {
	case QTPythonTerminalView::BeginningOfLine:
		m_offset = -m_command.length();
		refresh(false);
		break;
	case QTPythonTerminalView::EndOfLine:
		m_offset = 0;
		refresh(false);
		break;
	case QTPythonTerminalView::ForwardChar:
		if (++m_offset > 0)
			m_offset = 0;
		refresh(false);
		break;
	case QTPythonTerminalView::BackwardChar:
		if (--m_offset < -m_command.length())
			m_offset = -m_command.length();
		refresh(false);
		break;
	case QTPythonTerminalView::ClearScreen:
		clear();
		refresh(true);
		break;
	case QTPythonTerminalView::PreviousHistory:
		if (++m_history >= m_commandHistory.count())
			m_history = m_commandHistory.count() - 1;
		if (m_history == 0 && m_command.length() > 0) {
			m_commandHistory.prepend(m_command);
			++m_history;
		}
		if (m_history >= 0)
			m_command = m_commandHistory.at(m_history);
		refresh(false);
		break;
	case QTPythonTerminalView::NextHistory:
		if (--m_history < -1) {
			if (m_command.length() > 0)
				m_commandHistory.prepend(m_command);
			m_history = -1;
		}
		if (m_history >= 0)
			m_command = m_commandHistory.at(m_history);
		else
			m_command = "";
		refresh(false);
		break;
	case QTPythonTerminalView::DeleteChar:
		m_command.remove(m_command.length() + m_offset, 1);
		if (++m_offset > 0)
			m_offset = 0;
		refresh(false);
		break;
	case QTPythonTerminalView::BackwardDeleteChar:
		m_command.remove(m_command.length() + m_offset - 1, 1);
		refresh(false);
		break;
	case QTPythonTerminalView::KillLine:
		m_command.chop(-m_offset);
		m_offset = 0;
		refresh(false);
		break;
	case QTPythonTerminalView::Execute:
		exec(m_command);
		break;
	}
}

#define KEY_CONTROL (1 << 0)
#define KEY_SHIFT   (1 << 1)
#define KEY_ALT     (1 << 2)
#define KEY_META    (1 << 3)

void QTPythonTerminalView::keyReleaseEvent(QKeyEvent *ev)
{
	switch (ev->key()) {
	case Qt::Key_Control:
		m_modifiers &= ~KEY_CONTROL;
		break;
	case Qt::Key_Shift:
		m_modifiers &= ~KEY_SHIFT;
		break;
	case Qt::Key_Alt:
		m_modifiers &= ~KEY_ALT;
		break;
	case Qt::Key_Meta:
		m_modifiers &= ~KEY_META;
		break;
	}
	QPlainTextEdit::keyReleaseEvent(ev);
}

struct KeyBinding {
	int key;
	uint8_t modifiers;
	QTPythonTerminalView::KeyCommand command;
} keyBindings[] = {
	{Qt::Key_Left,     0, QTPythonTerminalView::BackwardChar},
	{Qt::Key_Right,    0, QTPythonTerminalView::ForwardChar},
	{Qt::Key_Up,       0, QTPythonTerminalView::PreviousHistory},
	{Qt::Key_Down,     0, QTPythonTerminalView::NextHistory},
	{Qt::Key_Home,     0, QTPythonTerminalView::BeginningOfLine},
	{Qt::Key_End,      0, QTPythonTerminalView::EndOfLine},
	{Qt::Key_Enter,    0, QTPythonTerminalView::Execute},
	{Qt::Key_Return,   0, QTPythonTerminalView::Execute},
	{Qt::Key_Escape,   0, QTPythonTerminalView::KillLine},
	{Qt::Key_Backspace,0, QTPythonTerminalView::BackwardDeleteChar},
	{Qt::Key_Delete,   0, QTPythonTerminalView::DeleteChar},
	{Qt::Key_A, KEY_CONTROL, QTPythonTerminalView::BeginningOfLine},
	{Qt::Key_E, KEY_CONTROL, QTPythonTerminalView::EndOfLine},
	{Qt::Key_F, KEY_CONTROL, QTPythonTerminalView::ForwardChar},
	{Qt::Key_B, KEY_CONTROL, QTPythonTerminalView::BackwardChar},
	{Qt::Key_L, KEY_CONTROL, QTPythonTerminalView::ClearScreen},
	{Qt::Key_P, KEY_CONTROL, QTPythonTerminalView::PreviousHistory},
	{Qt::Key_N, KEY_CONTROL, QTPythonTerminalView::NextHistory},
	{Qt::Key_D, KEY_CONTROL, QTPythonTerminalView::DeleteChar},
	{Qt::Key_K, KEY_CONTROL, QTPythonTerminalView::KillLine}
};

void QTPythonTerminalView::keyPressEvent(QKeyEvent *ev)
{
	if (ev->matches(QKeySequence::Copy)) {
		copy();
		return;
	} else if (ev->matches(QKeySequence::Cut)) {
		// no cut allowed, copy instead.
		copy();
		return;
	} else if (ev->matches(QKeySequence::Paste)) {
		paste();
		return;
	}

	switch (ev->key()) {
	case Qt::Key_Control:
		m_modifiers |= KEY_CONTROL;
		return;
	case Qt::Key_Shift:
		m_modifiers |= KEY_SHIFT;
		return;
	case Qt::Key_Alt:
		m_modifiers |= KEY_ALT;
		return;
	case Qt::Key_Meta:
		m_modifiers |= KEY_META;
		return;
	}

	int nBindings = sizeof(keyBindings)/sizeof(keyBindings[0]);
	for (int i = 0; i < nBindings; ++i) {
		if (keyBindings[i].key != ev->key())
			continue;
		if (keyBindings[i].modifiers != m_modifiers)
			continue;
		keyCommand(keyBindings[i].command);
		return;
	}

	if (!(m_modifiers & ~KEY_SHIFT) && ev->text().length() >= 1) {
		m_command.insert(m_command.length() + m_offset, ev->text());
		refresh(false);
	}
}
