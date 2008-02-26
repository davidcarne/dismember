#ifndef _documentwindow_h_
#define _documentwindow_h_

#include <wx/wx.h>
#include <wx/laywin.h>
#include <wx/vscroll.h>
#include <wx/listctrl.h>
#include <wx/fontdlg.h>
#ifdef WXAUI
#include "wxaui.h"
typedef wxFrameManager wxAuiManager;
#else
#include <wx/aui/aui.h>
#endif

#include <stack>
#include <vector>
#include <map>

#include "guiproxy.h"
#include "guiglue.h"
#include "wxguiutils.h"

class DataTypeListView;
class CodeViewCanvas;
class ToolBar;
class SymbolListView;
class MemoryLayoutView;
class DataTypeListView;
class PythonTerminalView;

class Document;

/**
 * Class that represents the root window for a document
 */
class DocumentWindow : public wxFrame, public DocumentGui
{
public:
	~DocumentWindow();
	
	void OnQuit(wxCommandEvent& event);
	void OnGoto(wxCommandEvent& event);
	void OnFont(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent & event);
	void OnLoadAuto(wxCommandEvent & event);
	
	void OnUpdateAll(wxCommandEvent & event);
	void OnEventTimer(wxTimerEvent& event);
	
	void UpdateAll();
	virtual void postUpdate();
	
	SymbolListView *m_routines;
	CodeViewCanvas *m_canvas;
	MemoryLayoutView *m_dataview;
	DataTypeListView *m_datatypelist;
	PythonTerminalView * m_pythonTerminal;
	
	
	DECLARE_EVENT_TABLE()
private:
	
	wxTimer *m_eventFilterTimer;
	bool m_onTimeOut;
	bool m_eventAccuum;
	
	DocumentWindow(const wxString& title, Document & doc);
	
	friend DocumentGui * setupDocumentGui(Document & doc);

	Document & m_doc;
	std::map <int, std::string> m_loaders_map;
	
	wxAuiManager m_winmanager;
	std::list< wxWindow * > m_windows;
};

extern const wxChar *ALL_FILETYPES;
extern const wxChar *ARMTRACE_FILETYPES;

#endif
