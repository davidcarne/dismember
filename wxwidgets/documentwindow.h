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
	
	/**
	 * Notify docwindow that an update is available - may update immediately, or at next filter event
	 * if updates are being rate-limited
	 */
	virtual void postUpdate();

	/**
	 * Accessor for the datatypelist view
	 */
	DataTypeListView * getDataTypeListView() {
		return m_datatypelist;
	}
	
	
	/**
	 * Accessor for the datatypelist view
	 */
	CodeViewCanvas * getCodeListingView() {
		return m_canvas;
	}
	
	DECLARE_EVENT_TABLE()
private:
	
	/**
	 * Force immediate window update, not recommended
	 */
	void UpdateAll();
	
	/**
	 * Quit event sent, via menuitem or cmd-q
	 */
	void OnQuit(wxCommandEvent& event);
	
	/**
	 * Goto event sent - via menuitem
	 */
	void OnGoto(wxCommandEvent& event);
	
	/**
	 * Change font event sent - do we use this?
	 */
	void OnFont(wxCommandEvent& event);
	
	/**
	 * Save event sent
	 */
	void OnSave(wxCommandEvent& event);
	
	/**
	 * Load event sent
	 */
	void OnLoad(wxCommandEvent & event);
	
	/**
	 * Autoload event sent
	 */
	void OnLoadAuto(wxCommandEvent & event);
	
	/**
	 * Event to force immediate window update
	 */
	void OnUpdateAll(wxCommandEvent & event);
	
	/**
	 * Timer event used to filter window update
	 */
	void OnEventTimer(wxTimerEvent& event);
	

	
	SymbolListView *m_routines;
	CodeViewCanvas *m_canvas;
	MemoryLayoutView *m_dataview;
	DataTypeListView *m_datatypelist;
	PythonTerminalView * m_pythonTerminal;
	
	
	
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
