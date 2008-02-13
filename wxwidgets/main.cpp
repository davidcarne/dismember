#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.h"
#include "codeview.h"
#include "ids.h"

// TODO: remove this
#include "arch/arm/arm.h"

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
	wxFileDialog* openFileDialog = new wxFileDialog( NULL, _("Open file"), _T(""), _T(""), ARMTRACE_FILETYPES, wxOPEN, wxDefaultPosition);
	
	if (openFileDialog->ShowModal() == wxID_OK) {
		Trace *trace = Trace::load_project_file(openFileDialog->GetPath().fn_str());
		if (trace)
		{
			CodeView *codeview_frame = new CodeView(openFileDialog->GetPath(), *trace);
			codeview_frame->Show(true);
		}
	} 
}

void ArmTraceApp::OnNew(wxCommandEvent & WXUNUSED(event))
{
	Trace * trace = new Trace(new ARMArchitecture());
	CodeView *codeview_frame = new CodeView(_T("Untitled"), *trace);
	codeview_frame->Show(true);
}

bool ArmTraceApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	
	Trace * trace = new Trace(new ARMArchitecture());
	CodeView *codeview_frame = new CodeView(_T("Untitled"), *trace);
	codeview_frame->Show(true);
	
	return true;
}
