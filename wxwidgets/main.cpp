#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <stdexcept>

#include "main.h"
#include "documentwindow.h"
#include "document.h"
#include "ids.h"

#include "../app_main.h"

IMPLEMENT_APP(ArmTraceWXApp)

BEGIN_EVENT_TABLE(ArmTraceWXApp, wxApp)
EVT_MENU(ID_Open, ArmTraceWXApp::OnOpen)
EVT_MENU(ID_New, ArmTraceWXApp::OnNew)
EVT_BUTTON(ID_ToolOpen, ArmTraceWXApp::OnOpen)
END_EVENT_TABLE()

const wxChar *ALL_FILETYPES = _T(
									"All Filetypes|*"
									);
const wxChar *ARMTRACE_FILETYPES = _T(
								 "Armtrace Compressed Project Files|*.ap.bz2"
								 );
void ArmTraceWXApp::OnOpen(wxCommandEvent & WXUNUSED(event))
{
	
	throw new std::runtime_error("Open not yet implemented");
	/*
	wxFileDialog* openFileDialog = new wxFileDialog( NULL, _("Open file"), _T(""), _T(""), ARMTRACE_FILETYPES, wxOPEN, wxDefaultPosition);
	
	
	if (openFileDialog->ShowModal() == wxID_OK) {
		Trace *trace = Trace::load_project_file(openFileDialog->GetPath().fn_str());
		if (trace)
		{
			DocumentWindow *codeview_frame = new DocumentWindow(openFileDialog->GetPath(), *trace);
			codeview_frame->Show(true);
		}
	} */
	
}

void ArmTraceWXApp::OnNew(wxCommandEvent & WXUNUSED(event))
{
	Document * d = new Document();
}

bool ArmTraceWXApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	
	app_main();

	wxCommandEvent dummy;
	OnNew(dummy);
	
	return true;
}
