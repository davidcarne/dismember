/*
 *  pythonterminalview.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "pythonterminalview.h"
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/font.h>
#include <wx/textctrl.h>
#include <boost/python/errors.hpp>

#include "wxguiutils.h"
#include "documentwindow.h"

PythonTerminalView::PythonTerminalView(DocumentWindow * frame, LocalPythonInterpreter *lpy) : 
wxPanel(frame, wxID_ANY, wxDefaultPosition, wxSize(50, 50)), m_lpy(lpy), m_docwin(frame)
{
	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	
	m_output = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), wxRE_READONLY|wxVSCROLL);
	sz->Add(m_output, 1,wxEXPAND|wxALL, 2);

	m_commandLine = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(-1,-1), wxTE_PROCESS_ENTER);
	sz->Add(m_commandLine, 0,wxEXPAND |wxALL, 2);
	
	wxRichTextCtrl& r = *m_output;
	wxFont textFont = wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL);
	r.SetFont(textFont);
	
	/*r.BeginSuppressUndo();
	r.BeginTextColour(wxColour(255, 0, 0));
    r.WriteText(wxT("colour, like this red bit."));
    r.EndTextColour();
	r.EndSuppressUndo();
	r.Show(true);*/

	wxCaret * c = new wxCaret(m_output, 0,0);
	c->Hide();
	
	// Hide the caret
	r.SetCaret(c);
	
	SetSizer( sz ); 
	sz->SetSizeHints( this );
	sz->Show(true);
	
	
	
	
	// Point stderr / stdout our way
	lpy->getStderrRedirector()->setOutputAcceptor(this);
	lpy->getStdoutRedirector()->setOutputAcceptor(this);
}


void PythonTerminalView::write(const std::string & data, int stream)
{
	m_output->SetCaretPosition(m_output->GetLastPosition());
	m_output->BeginSuppressUndo();
	if (stream == 2)
		m_output->BeginTextColour(wxColour(255, 0, 0));
	

	m_output->AppendText(_U(data.c_str()));

	if (stream == 2)
		m_output->EndTextColour();
	
	m_output->EndSuppressUndo();
	m_output->ScrollPages(100);
	m_output->Refresh();
}
void PythonTerminalView::OnTextEnter(wxCommandEvent & WXUNUSED(m))
{
	wxString v = m_commandLine->GetValue();
	

	m_output->AppendText(_T(">>> "));
	m_output->BeginBold();
	m_output->AppendText(_U(v));
	m_output->AppendText(_T("\n"));
	m_output->EndBold();
	
	const char * cstr = v.fn_str();
	try {
		boost::python::object result = m_lpy->execsingle(cstr);
	} catch( boost::python::error_already_set ) {
		PyErr_Print();
	}
		
		
	m_output->ShowPosition(m_output->GetLastPosition());
	m_output->Refresh();
	
	m_commandLine->Clear();
	m_docwin->postUpdate();
}

BEGIN_EVENT_TABLE(PythonTerminalView, wxPanel)
EVT_TEXT_ENTER(wxID_ANY, PythonTerminalView::OnTextEnter)
END_EVENT_TABLE()


