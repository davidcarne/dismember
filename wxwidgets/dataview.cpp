#include "documentwindow.h"
#include "ids.h"
#include "../memlocdata.h"
#include "memsegment.h"
#include "document.h"
#include "codeviewcanvas.h"
#include "dataview.h"

#define DATACOLOR wxColour(0xb9,0x47,0x1b)
#define CODECOLOR wxColour(0x2d,0x36,0xbc)
#define UNKCOLOR  wxColour(0xbc,0xb3,0x2d)
#define SEGCOLOR  wxColour(0x22,0x66,0x22)

MemoryLayoutView::MemoryLayoutView(wxFrame *frame, Document & doc)
 : wxWindow(frame, wxID_ANY, wxDefaultPosition, wxSize(-1, 15)), m_doc(doc), m_trace(*doc.getTrace())
{
	m_parent = frame;
	Update();
}

#define ROUND(x) ((int)((x)+0.5f))

void MemoryLayoutView::Update()
{
	wxPaintEvent dummy;
	OnPaint(dummy);
}

void MemoryLayoutView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	float bytesperpix;
	wxPaintDC dc(this);
	wxCoord w, h;
	address_t mem_size = 0;
	GetSize(&w, &h);

	address_t firstaddr;
	
	if (m_trace.memsegs_begin() == m_trace.memsegs_end())
		return;
	
	// Setup the pixel->address map
	m_pixel_addr_map.clear();
	m_pixel_addr_map.resize(w);
	
	// Start address
	firstaddr = (*m_trace.memsegs_begin())->get_start();
	
	// Calculate the total bytecount of the file
	for (Trace::memseglist_ci mi = m_trace.memsegs_begin(); mi != m_trace.memsegs_end(); mi++)
	{
		mem_size += (*mi)->get_length();
	}
	
	bytesperpix = (float)mem_size / w;
	
	Trace::memseglist_ci mi = m_trace.memsegs_begin();
	double addr = (*mi)->get_start();
	
	for (wxCoord i = 0; i < w; ++i) {
		assert(mi != m_trace.memsegs_end());
		
		address_t norm_addr = ROUND(addr);
		MemlocData *id = m_trace.lookup_memloc(norm_addr, false);
		
		if (id) {
			if (id->is_executable())
				dc.SetPen(CODECOLOR);
			else
				dc.SetPen(DATACOLOR);
		}
		else
			dc.SetPen(UNKCOLOR);
		dc.DrawLine(i, 0, i, h);
		
		m_pixel_addr_map[i] = norm_addr;
		
		addr += bytesperpix;
		
		MemSegment * mseg = (*mi);
		if (addr > mseg->get_start() + mseg->get_length())
		{
			
			dc.SetPen(SEGCOLOR);
			dc.DrawLine(i, 0, i, h/4);
			mi++;
		}
	}
}

void MemoryLayoutView::OnClick(wxMouseEvent& m)
{
	if (m_pixel_addr_map.size() == 0)
		return;
	
	((DocumentWindow *)m_parent)->m_canvas->jumpToAddr(m_pixel_addr_map[m.GetX()]);
	m.Skip();
}

BEGIN_EVENT_TABLE(MemoryLayoutView, wxWindow)
EVT_PAINT(MemoryLayoutView::OnPaint)
EVT_LEFT_DOWN(MemoryLayoutView::OnClick)
END_EVENT_TABLE()
