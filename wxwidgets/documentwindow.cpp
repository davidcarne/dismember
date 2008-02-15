#include <math.h>
#include <stdexcept>
#include <wx/utils.h>
#include <wx/aui/aui.h>

#include "documentwindow.h"

#include "ids.h"
#include "../loaders/loaderfactory.h"
#include "datatypelist.h"
#include "codeviewcanvas.h"
#include "toolbar.h"
#include "routinelist.h"
#include "dataview.h"
#include "document.h"
#include "memsegment.h"
#include "pythonterminalview.h"

DocumentWindow::DocumentWindow(const wxString& title, Document & doc) : 
	wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), m_doc(doc)
{	 
	wxCommandEvent dummy;
	wxMenu *menufile = new wxMenu;
	
	menufile->Append(ID_New, _T("New...\tCTRL+n"));
	menufile->Append(ID_Open, _T("Open...\tCTRL+o"));
	menufile->Append(ID_Save, _T("Save...\tCTRL+s"));
	menufile->Append(ID_Close, _T("Quit\tCTRL+q"));
	
	wxMenu *menuedit = new wxMenu;
	menuedit->Append(ID_Font, _T("Font...\tCTRL+t"));
	menuedit->Append(ID_GotoAddress, _T("Goto Address..."));
	menuedit->Append(ID_LoadAuto, _T("AutoLoad...\tCTRL+l"));
	
	wxMenu * menuLoadTypes = new wxMenu;
	wxRegisterId(7000);
	
	FileLoaderMaker::factories_ci loaders_ind = FileLoaderMaker::getMap().begin();
	FileLoaderMaker::factories_ci loaders_end = FileLoaderMaker::getMap().end();
	
	for (;loaders_ind != loaders_end; loaders_ind++)
	{
		int  i = wxNewId();
		m_loaders_map[i] = (*loaders_ind).first;
		menuLoadTypes->Append(i, _U((*loaders_ind).first.c_str()));
		Connect(i,wxEVT_COMMAND_MENU_SELECTED,
				wxCommandEventHandler(DocumentWindow::OnLoad));
	}
	
	menuedit->Append(wxID_ANY, _T("Load"), menuLoadTypes);
	
	//wxMenu *menuview = new wxMenu;
	wxMenu * menuview_defcons = new wxMenu;
	menuedit->Append(wxID_ANY, _T("Default constant type"), menuview_defcons);
	
	wxMenuBar *menubar = new wxMenuBar;
	menubar->Append(menufile, _T("File"));
	menubar->Append(menuedit, _T("Edit"));
	menubar->Append(menuedit, _T("View"));

	SetMenuBar(menubar);
	CreateStatusBar(1);

	m_windows.empty();
#ifdef WXAUI
	m_winmanager.SetFrame(this);
#else
	m_winmanager.SetManagedWindow(this);
#endif

	std::list< wxWindow * >::iterator it = m_windows.begin();
	for (; it != m_windows.end(); it++) {
		wxWindow *wxw = *it;
		m_winmanager.DetachPane(wxw);
		wxw->Destroy();
	}
	m_windows.empty();
	
	m_canvas = new CodeViewCanvas(this, doc);
	m_routines = new SymbolListView(this, doc);
	TopToolBar *tb = new TopToolBar(this);
	m_dataview = new MemoryLayoutView(this, doc);
	m_datatypelist = new DataTypeListView(this, doc);
	
	// TODO: save me
	m_pythonTerminal = new PythonTerminalView(this, doc.getPythonInterpreter());
	
	m_winmanager.AddPane(m_routines, wxLEFT, _T("Routines"));
	m_winmanager.AddPane(m_datatypelist, wxRIGHT, _T("Data Types"));
	m_winmanager.AddPane(tb, wxTOP);
	m_winmanager.GetPane(tb).ToolbarPane();
	m_winmanager.AddPane(m_dataview, wxAuiPaneInfo().Bottom().Row(2));
	m_winmanager.AddPane(m_pythonTerminal, wxAuiPaneInfo().Bottom().Row(1).Name(_T("Python Terminal")).MinSize(wxSize(-1,200)));
	m_winmanager.GetPane(m_dataview).CaptionVisible(0).Gripper(1);
	m_winmanager.AddPane(m_canvas, wxCENTER);
	
	m_windows.push_back(m_routines);
	m_windows.push_back(m_canvas);
	m_windows.push_back(tb);
	m_windows.push_back(m_dataview);
	m_windows.push_back(m_datatypelist);

	m_winmanager.Update();
	
	SetStatusText(title, 0);


	Centre();
}

DocumentWindow::~DocumentWindow()
{
	m_winmanager.UnInit();
}

void DocumentWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	this->Destroy();
}

void DocumentWindow::OnGoto(wxCommandEvent& event)
{
	m_canvas->OnGoto(event);
}

void DocumentWindow::OnFont(wxCommandEvent& event)
{
	m_canvas->OnFont(event);
}

void DocumentWindow::OnSave(wxCommandEvent& event)
{
	throw new std::runtime_error("Open not yet implemented");
	/*
	wxFileDialog* saveFileDialog = new wxFileDialog( this, _("Save File.."), _T(""), _T(""), ARMTRACE_FILETYPES, wxSAVE, wxDefaultPosition);
	
	if (saveFileDialog->ShowModal() == wxID_OK) {
		m_ctx.write_file(saveFileDialog->GetPath().fn_str());
	}*/
}

void DocumentWindow::OnLoadAuto(wxCommandEvent & event)
{	
	wxFileDialog* openFileDialog = new wxFileDialog( NULL, _("Load file"), _T(""), _T(""), ALL_FILETYPES, wxOPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK) {
		
		FILE *loadimg;
		if (!(loadimg = fopen(openFileDialog->GetPath().fn_str(), "r")))
			return;
		
		if (!FileLoaderMaker::autoLoadFromFile(loadimg, m_doc.getTrace()))
		{
			wxMessageBox(_T("Automatic File load Failed!"),
					_T("File load Failed!"));
		}
		
		fclose(loadimg);
	} 
	
	// This should be encapsulated in the Update/refresh method for codeview
	UpdateAll();
}

void DocumentWindow::OnLoad(wxCommandEvent & event)
{
	int id = event.GetId();
	std::map<int, std::string>::const_iterator l = m_loaders_map.find(id);
	std::string loader_name = (*l).second;

	
	wxFileDialog* openFileDialog = new wxFileDialog( NULL, _("Load file"), _T(""), _T(""), ALL_FILETYPES, wxOPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK) {

		const char * fstr = openFileDialog->GetPath().fn_str();
		
		FILE *loadimg;
		if (!(loadimg = fopen(fstr, "r")))
			return;
		
		if (!FileLoaderMaker::loadFromFile(loader_name, loadimg, m_doc.getTrace()))
		{
			wxMessageBox(_T("File load Failed!"), _T("File load Failed!"));
		}
		
		fclose(loadimg);
	} 
	
	// This should be encapsulated in the Update/refresh method for codeview
	UpdateAll();
}

void DocumentWindow::UpdateAll()
{
	m_canvas->updateLines();
	m_canvas->RefreshAll();
	m_routines->Update();
	m_dataview->Refresh();
}

BEGIN_EVENT_TABLE(DocumentWindow, wxFrame)
EVT_MENU(ID_Save, DocumentWindow::OnSave)
EVT_MENU(ID_Close, DocumentWindow::OnQuit)
EVT_MENU(ID_GotoAddress, DocumentWindow::OnGoto)
EVT_MENU(ID_Font, DocumentWindow::OnFont)
EVT_MENU(ID_LoadAuto, DocumentWindow::OnLoadAuto)
EVT_BUTTON(ID_ToolClose, DocumentWindow::OnQuit)
END_EVENT_TABLE()

