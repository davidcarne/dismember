#include "codeview.h"
#include <math.h>
#include "ids.h"
#include "../loaders/loaderfactory.h"
#include "datatypelist.h"
#include "codeviewcanvas.h"
#include "toolbar.h"
#include "routinelist.h"
#include "dataview.h"

#include "memsegment.h"

#include <wx/utils.h>

CodeView::CodeView(const wxString& title, Trace & ctx) : 
	wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), m_ctx(ctx)
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
				wxCommandEventHandler(CodeView::OnLoad));
	}
	
	menuedit->Append(wxID_ANY, _T("Load"), menuLoadTypes);
	
	wxMenu *menuview = new wxMenu;
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
	
	m_canvas = new CodeViewCanvas(this, ctx);
	m_routines = new RoutineListView(this, ctx);
	TopToolBar *tb = new TopToolBar(this);
	m_dataview = new DataView(this, ctx);
	m_datatypelist = new DataTypeList(this, ctx);
	
	m_winmanager.AddPane(m_routines, wxLEFT, _T("Routines"));
	m_winmanager.AddPane(m_datatypelist, wxRIGHT, _T("Data Types"));
	m_winmanager.AddPane(tb, wxTOP);
	m_winmanager.GetPane(tb).ToolbarPane();
	m_winmanager.AddPane(m_dataview, wxBOTTOM);
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

CodeView::~CodeView()
{
	m_winmanager.UnInit();
}

void CodeView::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	this->Destroy();
}

void CodeView::OnGoto(wxCommandEvent& event)
{
	m_canvas->OnGoto(event);
}

void CodeView::OnFont(wxCommandEvent& event)
{
	m_canvas->OnFont(event);
}

void CodeView::OnSave(wxCommandEvent& event)
{
	wxFileDialog* saveFileDialog = new wxFileDialog( this, _("Save File.."), _T(""), _T(""), ARMTRACE_FILETYPES, wxSAVE, wxDefaultPosition);
	
	if (saveFileDialog->ShowModal() == wxID_OK) {
		m_ctx.write_file(saveFileDialog->GetPath().fn_str());
	}
}

void CodeView::OnLoadAuto(wxCommandEvent & event)
{	
	wxFileDialog* openFileDialog = new wxFileDialog( NULL, _("Load file"), _T(""), _T(""), ALL_FILETYPES, wxOPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK) {
		
		FILE *loadimg;
		if (!(loadimg = fopen(openFileDialog->GetPath().fn_str(), "r")))
			return;
		
		if (!FileLoaderMaker::autoLoadFromFile(loadimg, &m_ctx))
		{
			wxMessageBox(_T("Automatic File load Failed!"),
					_T("File load Failed!"));
		}
		
		fclose(loadimg);
	} 
	
	// This should be encapsulated in the Update/refresh method for codeview
	m_canvas->updateLines();
	m_canvas->RefreshAll();
	
	m_routines->Update();
	m_dataview->Refresh();
}

void CodeView::OnLoad(wxCommandEvent & event)
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
		
		if (!FileLoaderMaker::loadFromFile(loader_name, loadimg, &m_ctx))
		{
			wxMessageBox(_T("File load Failed!"), _T("File load Failed!"));
		}
		
		fclose(loadimg);
	} 
	
	// This should be encapsulated in the Update/refresh method for codeview
	m_canvas->updateLines();
	m_canvas->RefreshAll();
	m_routines->Update();
	m_dataview->Refresh();
}

BEGIN_EVENT_TABLE(CodeView, wxFrame)
EVT_MENU(ID_Save, CodeView::OnSave)
EVT_MENU(ID_Close, CodeView::OnQuit)
EVT_MENU(ID_GotoAddress, CodeView::OnGoto)
EVT_MENU(ID_Font, CodeView::OnFont)
EVT_MENU(ID_LoadAuto, CodeView::OnLoadAuto)
EVT_BUTTON(ID_ToolClose, CodeView::OnQuit)
END_EVENT_TABLE()

