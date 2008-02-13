#ifndef codeview_h
#define codeview_h

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

#include "trace.h"
#include "guiproxy.h"

#include <stack>
#include <vector>
#include <map>

#if defined(wxUSE_UNICODE) && !defined(_U)
#define _U(x) wxString((x),wxConvUTF8)
#elif !defined(_U)
#define _U(x) (x)
#endif

class DataTypeList;
class CodeViewCanvas;
class ToolBar;
class RoutineListView;
class DataView;
class DataTypeList;

class CodeView : public wxFrame
{
public:
	CodeView(const wxString& title, Trace &ctx);
	~CodeView();
	
	void OnQuit(wxCommandEvent& event);
	void OnGoto(wxCommandEvent& event);
	void OnFont(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent & event);
	void OnLoadAuto(wxCommandEvent & event);
	RoutineListView *m_routines;
	CodeViewCanvas *m_canvas;
	DataView *m_dataview;
	DataTypeList *m_datatypelist;
	
	DECLARE_EVENT_TABLE()
private:
	Trace & m_ctx;
	std::map <int, std::string> m_loaders_map;
	
	wxAuiManager m_winmanager;
	std::list< wxWindow * > m_windows;
};

extern const wxChar *ALL_FILETYPES;
extern const wxChar *ARMTRACE_FILETYPES;

#endif
