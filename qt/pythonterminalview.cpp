#include <QFontMetrics>
#include "pythonterminalview.h"

QTPythonTerminalView::QTPythonTerminalView(QWidget *parent)
 : QPlainTextEdit(parent), m_offset(0), m_history(-1)
{
	setCursorWidth(fontMetrics().maxWidth());
	setTabStopWidth(fontMetrics().maxWidth()*4);
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

void QTPythonTerminalView::keyPressEvent(QKeyEvent *ev)
{
	LocalPythonInterpreter *lpy = m_runtime->getRuntime().getPythonInterpreter();
	switch (ev->key()) {
	case Qt::Key_Left:
		if (--m_offset < -m_command.length())
			m_offset = -m_command.length();
		refresh(false);
		break;
	case Qt::Key_Right:
		if (++m_offset > 0)
			m_offset = 0;
		refresh(false);
		break;
	case Qt::Key_Up:
		if (++m_history >= m_commandHistory.count()) {
			m_history = m_commandHistory.count() - 1;
		}
		if (m_history >= 0)
			m_command = m_commandHistory.at(m_history);
		refresh(false);
		break;
	case Qt::Key_Down:
		if (--m_history < -1)
			m_history = -1;
		if (m_history >= 0)
			m_command = m_commandHistory.at(m_history);
		else
			m_command = "";
		refresh(false);
		break;
	case Qt::Key_Home:
		m_offset = -m_command.length();
		refresh(false);
		break;
	case Qt::Key_End:
		m_offset = 0;
		refresh(false);
		break;
	case Qt::Key_PageUp:
	case Qt::Key_PageDown:
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
		try {
			QByteArray ba = m_command.toLocal8Bit();
			write("\n", 1);
			m_commandHistory.prepend(QString(m_command));
			boost::python::object res = lpy->execsingle(ba.constData());
		} catch (boost::python::error_already_set &) {
			PyErr_Print();
		}
		m_offset = 0;
		m_command = "";
		m_history = -1;
		refresh(true);
		break;
	case Qt::Key_Escape:
		m_command = "";
		refresh(false);
		break;
	case Qt::Key_Backspace:
		m_command.remove(m_command.length() + m_offset - 1, 1);
		refresh(false);
		break;
	case Qt::Key_Delete:
		m_command.remove(m_command.length() + m_offset, 1);
		if (++m_offset > 0)
			m_offset = 0;
		refresh(false);
		break;
	case Qt::Key_Tab:
	default:
		if (ev->text().length() >= 1) {
			m_command.insert(m_command.length() + m_offset,
				ev->text());
			refresh(false);
		}
		break;
	}
}
