/*
 *  pythonterminalview.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _pythonterminalview_H_
#define _pythonterminalview_H_
class LocalPythonInterpreter;
class wxRichTextCtrl;
class wxTextCtrl;

#include "localpythoninterpreter.h"

class DocumentWindow;
#include <wx/wx.h>
/**
 * Panel implementing a basic python terminal
 */
class PythonTerminalView: public wxPanel, public LPIOutputStreamAcceptor
{
public:
	/**
	 * Construct a new panel implementing a python terminal, given the overall frame for update notification
	 * and the local python interpreter we're drawing
	 * @param frame DocumentWindow to call updateall on when we do something
	 * @param lpy Local Python interpreter
	 */
	PythonTerminalView(DocumentWindow * frame, LocalPythonInterpreter *lpy);
	
	/**
	 * method to write text to the terminal; called by the python interpreter
	 * @param data the data to write to the terminal
	 * @param stream 1 - stdout, 2 - stderr
	 */
	virtual void write(const std::string & data, int stream);
	
	/**
	 * Event handler for enter-press on the text submit box
	 */
	void OnTextEnter(wxCommandEvent & m);

private:
	wxRichTextCtrl* m_output;
	wxTextCtrl *m_commandLine;
	
	LocalPythonInterpreter * m_lpy;
	
	DocumentWindow * m_docwin;
public:
	DECLARE_EVENT_TABLE();
};



#endif


