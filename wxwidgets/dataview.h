#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_

class Document;
/**
 * MemoryLayoutView is a wxWidgets window that displays the current memory layout graphicly, with coloring indicating 
 * what kind of data has been decoded at that location
 */
class MemoryLayoutView: public wxWindow
{
public:
	/**
	 * Create a new MemoryLayoutView
	 */
	MemoryLayoutView(wxFrame *parent, Document &doc);
	
	/**
	 * Click event handler
	 */
	void OnClick(wxMouseEvent& m);
	
	/**
	 * Paint event handler; redraws the screen
	 */
	void OnPaint(wxPaintEvent& event);
	
	/**
	 * Notify the view that the document has changed, and it must recalculate.
	 */
	void Update();
	
	/**
	 * ignore me.
	 */
	DECLARE_EVENT_TABLE();
private:
	std::vector<address_t> m_pixel_addr_map;
	Document & m_doc;
	Trace &m_trace;
	wxFrame *m_parent;
};

#endif

