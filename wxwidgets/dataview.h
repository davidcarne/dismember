#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_

class DataView: public wxWindow
{
public:
	DataView(wxFrame *parent, Trace &ctx);
	void OnClick(wxMouseEvent& m);
	void OnPaint(wxPaintEvent& event);
	void Update();
	DECLARE_EVENT_TABLE();
private:
		std::vector<address_t> m_pixel_addr_map;
	
	Trace &m_trace;
	wxFrame *m_parent;
};

#endif