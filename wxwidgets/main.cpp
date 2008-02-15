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

IMPLEMENT_APP(ArmTraceApp)

BEGIN_EVENT_TABLE(ArmTraceApp, wxApp)
EVT_MENU(ID_Open, ArmTraceApp::OnOpen)
EVT_MENU(ID_New, ArmTraceApp::OnNew)
EVT_BUTTON(ID_ToolOpen, ArmTraceApp::OnOpen)
END_EVENT_TABLE()

const wxChar *ALL_FILETYPES = _T(
									"All Filetypes|*"
									);
const wxChar *ARMTRACE_FILETYPES = _T(
								 "Armtrace Compressed Project Files|*.ap.bz2"
								 );
void ArmTraceApp::OnOpen(wxCommandEvent & WXUNUSED(event))
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

void ArmTraceApp::OnNew(wxCommandEvent & WXUNUSED(event))
{
	Document * d = new Document();
}

bool ArmTraceApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	
	app_main();

	wxCommandEvent dummy;
	OnNew(dummy);
	
	return true;
}
