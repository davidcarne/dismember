#include <stdexcept>

#include "documentwindow.h"
#include "ids.h"
#include "codeviewcanvas.h"
#include "dataview.h"
#include "datatypelist.h"
#include "../memlocdata.h"
#include "../xref.h"
#include "document.h"

static wxFont G_font;
static int G_font_height;

void CodeViewCanvas::OnSymbolChange(wxCommandEvent& WXUNUSED(event))
{
	if (!m_is_sel) return;
	const Symbol * s= m_trace.lookup_symbol(m_sel_addr);
	wxString newsymname = wxGetTextFromUser(_T("enter a new symbol name"),
			_T("edit symbol"), s ? _U(s->get_name().c_str()) : _T(""));
	if (newsymname.Length() > 0)
		m_trace.create_sym((const char *)newsymname.fn_str(), m_sel_addr);
	Refresh();
}

static wxString wxGetMultilineTextFromUser(const wxString& message,
		const wxString& caption = _T("Input text"),
		const wxString& default_value = _T(""), wxWindow *parent = NULL,
		int x = wxDefaultCoord,	int y = wxDefaultCoord,
		bool centre = true)
{
	wxDialog dialog(parent, -1, caption, wxPoint(x, y));
	wxPanel *panel = new wxPanel(&dialog, -1);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxStaticText text(panel, -1, message);
	wxTextCtrl tc(panel, -1, default_value, wxDefaultPosition,
			wxSize(250, 150), wxTE_MULTILINE);

	vbox->Add(&text, 1, wxBOTTOM);
	vbox->Add(-1, 10);
	vbox->Add(&tc, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	vbox->Add(-1, 10);

	wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
	wxButton bu(panel, wxID_OK, _T("OK"));
	box->Add(&bu, 1);
	vbox->Add(box, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	panel->SetSizer(vbox);
	panel->Fit();
	dialog.SetClientSize(panel->GetSize());
	dialog.Centre();
	if (dialog.ShowModal() == wxID_OK) {
		wxString ret = _T("");
		for (int i = 0; i < tc.GetNumberOfLines(); ++i)
			ret += tc.GetLineText(i) + _T("\n");
		return ret.Mid(0, ret.Length() - 1);
	}
	
	return _U("");
}


void CodeViewCanvas::OnComment(wxCommandEvent& WXUNUSED(event))
{
	if (!m_is_sel) return;
	Comment *cmt = m_trace.lookup_comment(m_sel_addr);

	wxString comment = wxGetMultilineTextFromUser(_T("Edit comment"),
		_T("Edit comment"),
		cmt ? _U(cmt->get_comment().c_str()) : _T(""));

	if (comment.Length() > 0)
		m_trace.create_comment((char *)comment.c_str(),
				m_sel_addr);
	Refresh();
}

void CodeViewCanvas::OnAnalyze(wxCommandEvent& WXUNUSED(event))
{
	if (!m_is_sel) return;
	m_trace.analyze(m_sel_addr);
	((DocumentWindow *)m_parent)->m_dataview->Update();
	updateLines();
}

void CodeViewCanvas::OnUndefine(wxCommandEvent& WXUNUSED(event))
{
	if (!m_is_sel) return;
	//TODO: re-enable m_trace.undefine(m_sel_addr);
	((DocumentWindow *)m_parent)->m_dataview->Update();
	updateLines();
}

void CodeViewCanvas::jumpToAddr(address_t addr)
{
	//printf("scrolling to line %x\n", m_gprox->get_line_for_addr(addr));
	ScrollToLine(m_gprox->get_line_for_addr(addr));
	
	m_current_first_addr = m_gprox->get_addr_for_line(GetFirstVisibleLine());
	m_current_first_addr_set = true;
	Refresh();
}

void CodeViewCanvas::OnGoto(wxCommandEvent& WXUNUSED(event))
{
	wxString addr_str;
	u32 last_addr, cur_addr;
	bool should_save = true;
	
	if (m_is_sel)
		last_addr = cur_addr = m_sel_addr;
	else
		try {
			last_addr = cur_addr = m_gprox->get_addr_for_line(GetFirstVisibleLine());
		} catch(std::out_of_range e)
		{
			should_save = false;
			last_addr = 0;
		}
	
	addr_str = wxGetTextFromUser(_T("Goto address:"),_T("Goto address"),
			wxString::Format(_T("0x%08x"),
			(u32)last_addr));
	
	if (addr_str.Length() > 0)
		cur_addr = strtol(addr_str.fn_str(), 0,  0); 

	if (cur_addr != last_addr) {
		line_ind_t l;
		try {
			l = m_gprox->get_line_for_addr(cur_addr);
		} catch (std::out_of_range e) {
			// HACK - report failure gracefully
			return;
		}
		ScrollToLine(l);
		m_sel_addr = cur_addr;
		
		if (should_save)
		{
			m_addr_stack.push(last_addr);
			m_is_sel = true;
		}
	}
	Refresh();
}

void CodeViewCanvas::OnGotoBranch(wxCommandEvent& WXUNUSED(event))
{
	if (!m_is_sel) return;
	u32 addr = m_sel_addr;
	MemlocData * i = m_trace.lookup_memloc(addr);

	if (i && i->has_xrefs_from()) {
		Xref * x = (*(i->begin_xref_from())).second;
		
		u32 na = x->get_dst_addr();
		
		ScrollToLine(m_gprox->get_line_for_addr(na));
		m_addr_stack.push(m_sel_addr);
		m_is_sel = true;
		m_sel_addr = na;
	}
	Refresh();
}

void CodeViewCanvas::OnXREFs(wxCommandEvent & WXUNUSED(event))
{
	if (!m_is_sel) return;
	u32 addr = m_sel_addr;
	MemlocData *i = m_trace.lookup_memloc(addr);
	
	if (!i || !i->has_xrefs_to())
		return;
	
	if (i->count_xrefs_to() == 1) {
		address_t dest_addr = (*i->begin_xref_to()).second->get_src_addr();
		u32 line = m_gprox->get_line_for_addr(dest_addr);
		m_addr_stack.push(m_sel_addr);
		m_is_sel = true;
		m_sel_addr = dest_addr;
		
		ScrollToLine(line);
		Refresh();
		return;
	}
	wxArrayString xrefs;
	
	u32 * client = new u32[i->count_xrefs_to()];
	u32 * client_p = client;
	
	xref_map_ci xr = i->begin_xref_to();
		
	for (; xr != i->end_xref_to(); xr++)
	{
		Xref * xref = (*xr).second;
		if (xref->get_src_inst() && xref->get_src_inst()->get_symbol())
			xrefs.Add(_U(xref->get_src_inst()->get_symbol()->get_name().c_str()));
		else
			xrefs.Add(wxString::Format(_T("%08x"), xref->get_src_addr()));
		*client_p++ = xref->get_src_addr();
	}

		
	wxSingleChoiceDialog d(m_parent, _T("Select an XREF to jump to"),_T("Select an XREF to jump to"), xrefs, (char**)client);
	if (d.ShowModal() == wxID_OK)
	{
		u32 addr = (u32) d.GetSelectionClientData();
		u32 line = m_gprox->get_line_for_addr(addr);
		
		// save current line + select new
		m_addr_stack.push(m_sel_addr);
		m_is_sel = true;
		m_sel_addr = addr;
		
		ScrollToLine(line);
	}
	delete client;
}

void CodeViewCanvas::OnMouseDown(wxMouseEvent& m)
{
	s32 sel_line = HitTest(m.GetPosition());
	
	if (sel_line == -1)
		return;
	
	u32 sel_addr = m_gprox->get_addr_for_line(sel_line);
	
	if (m_is_sel && m_sel_addr == sel_addr)
		m_is_sel = false;
	else
	{
		m_is_sel = true;
		m_sel_addr = sel_addr;
	}
	Refresh();
	m.Skip();
	
}

void CodeViewCanvas::OnRightDown(wxMouseEvent& m)
{
	u32 sel_line = HitTest(m.GetPosition());
	if (sel_line == -1)
		return;
	
	u32 sel_addr = m_gprox->get_addr_for_line(sel_line);
	m_sel_addr = sel_addr;
	m_is_sel = true;
	
	MemlocData *i = m_trace.lookup_memloc(m_sel_addr);
		

	wxMenu *popup = new wxMenu;
	popup->Append(ID_ChangeSymbol, _T("Change symbol..."));
	
	if (i && i->has_xrefs_from())
		popup->Append(ID_GotoBranch, _T("Goto branch"));
	
	if (i && i->has_xrefs_to()) {
		if (i->count_xrefs_to() == 1)
			popup->Append(ID_GotoXREF,
				wxString::Format(_T("Goto XREF 0x%08x"),
					(*i->begin_xref_to()).second->get_src_addr()));
		else
			popup->Append(ID_GotoXREF, _T("Goto XREF..."));
	}
	
	if (!i)
		popup->Append(ID_Analyze, _T("Analyze from here"));

	Refresh();
	PopupMenu(popup, m.GetPosition());

	m.Skip();
}

void CodeViewCanvas::OnKeyDown(wxKeyEvent& k)
{
	switch (k.GetKeyCode()) {
	case WXK_RETURN: {
		wxCommandEvent dummy;
		OnGotoBranch(dummy);
		} break;

	case WXK_DOWN: {
		if (m_is_sel) {
			
			size_t sel_line = m_gprox->get_line_for_addr(m_sel_addr);
			if (sel_line < m_gprox->get_line_count() - 1)
				m_sel_addr = m_gprox->get_addr_for_line(sel_line + 1);
			
			if (sel_line + 1 > GetLastVisibleLine())
				ScrollLines(+1);
		}
		else
			ScrollLines(+1);
		} break;

	case WXK_UP: {
		if (m_is_sel) {
			
			size_t sel_line = m_gprox->get_line_for_addr(m_sel_addr);
			if (sel_line > 0)
				m_sel_addr = m_gprox->get_addr_for_line(sel_line - 1);
			
			if (sel_line - 1 < GetFirstVisibleLine())
				ScrollLines(-1);
		}
		else
			ScrollLines(-1);
	} break;

	case WXK_PAGEUP: {
		ScrollLines(GetFirstVisibleLine() - GetLastVisibleLine());
		} break;

	case WXK_PAGEDOWN: {
		ScrollLines(GetLastVisibleLine() - GetFirstVisibleLine());
		} break;

	case WXK_ESCAPE: {
		if (!m_addr_stack.empty()) {
			ScrollToLine(m_gprox->get_line_for_addr(m_addr_stack.top()));
			m_addr_stack.pop();
		}
		} break;

	case 'N': {
		wxCommandEvent dummy;
		OnSymbolChange(dummy);
		} break;

	case 'C': {
		wxCommandEvent dummy;
		OnAnalyze(dummy);
		} break;
	
	case 'U': {
		wxCommandEvent dummy;
		OnUndefine(dummy);
	} break;
		
	case 'G': {
		wxCommandEvent dummy;
		OnGoto(dummy);
		} break;

	case 'X': {
		wxCommandEvent dummy;
		OnXREFs(dummy);
	} break;

	case 'D': {
		wxCommandEvent dummy;
		OnCreateData(k.ShiftDown());
	} break;
		
	case 'T':
	case ';': {
		wxCommandEvent dummy;
		OnComment(dummy);
	} break;

	default:
		k.Skip();
		break;
	}
	Refresh();
}

void CodeViewCanvas::OnCreateData(bool directFromBox)
{
	if (directFromBox)
	{
		wxMouseEvent dummy;
		m_parent->m_datatypelist->createSelectedDataType();
		
	} else {
		
	}
}

#define LINEHEIGHT(x) ((x)+(x)/6)
void CodeViewCanvas::updateLines()
{
	SetLineCount(m_gprox->get_line_count());
	Refresh();
	
	if (m_gprox->get_line_count() != 0 && m_current_first_addr_set)
		ScrollToLine(m_gprox->get_line_for_addr(m_current_first_addr));
	RefreshAll();
}

void CodeViewCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	std::list<Comment *> comments;
	std::list<wxCoord> lines;



	wxPaintDC dc(this);
	if (!G_font.Ok())
	{
		G_font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T(""));
		dc.SetFont(G_font);
		G_font_height = dc.GetCharHeight();
	}
	dc.SetFont(G_font);
	dc.SetBackground(*wxWHITE);
	dc.Clear();
	dc.SetPen(*wxBLACK_DASHED_PEN);
	
	
	if (m_gprox->get_line_count() == 0)
		return;
	
	m_current_first_addr = m_gprox->get_addr_for_line(GetFirstVisibleLine());
	m_current_first_addr_set = true;
	
	const size_t lineFirst = GetFirstVisibleLine(), lineLast = GetLastVisibleLine();
	
	const wxCoord hText = dc.GetCharHeight();
	
	
	wxCoord y = 0;
	for ( size_t line = lineFirst; line <= lineLast; line++ )
	{
		u32 addr = m_gprox->get_addr_for_line(line);

		wxCoord hLine = OnGetLineHeight(line);
		
		size_t sel_line;
		
		if (m_is_sel)
			sel_line = m_gprox->get_line_for_addr(m_sel_addr);
		
		bool sel = m_is_sel && (sel_line == line);

		dc.SetPen(wxColour(0xEF,0xEF,0xEF));
		dc.SetBrush(wxColour(0xEF,0xEF,0xEF));
		if (line & 1)
			dc.DrawRectangle(0, y, GetRect().GetWidth(), hLine);
		
		dc.SetPen(wxColour(0xB0,0xB0,0xEF));
		dc.SetBrush(wxColour(0xC0,0xC0,0xFF));
		
		if (sel)
			dc.DrawRectangle(0, y, GetRect().GetWidth(), hLine);
		
		dc.SetTextForeground(*wxBLUE);
		dc.DrawText(wxString::Format(_T("%08x"), addr), 0, y + hLine - hText);
		

		const Symbol * sym = m_trace.lookup_symbol(addr);
		if (sym)
		{
			std::string symname = sym->get_name();
			dc.DrawText(_U(symname.c_str()), m_sym_offs, y + hLine - hText);
		}

		Comment *cmt = m_trace.lookup_comment(addr);
		if (cmt) {
			comments.push_back(cmt);
			lines.push_back(y);
		}
		
		MemlocData * id = m_trace.lookup_memloc(addr);
		if (id)
		{
			dc.SetTextForeground(*wxBLACK);
			
			// we need our own copy of the string. no really, HACK
			std::string dis_str = id->get_textual();
			const char * dis_txt = dis_str.c_str();
			
			char * args = strchr(dis_txt, '\t');
			if (args)
			{
				*args = 0;
				args++;
			}
			dc.DrawText(_U(dis_txt), m_disas_offs, y + hLine - hText);
			if (args)
				dc.DrawText(_U(args), m_disas_offs+50, y + hLine - hText);
			
			if (id->has_xrefs_to())
			{
				dc.SetTextForeground(*wxLIGHT_GREY);
				int nrefs = id->count_xrefs_to();
				
				for (xref_map_ci j = id->begin_xref_to(); j != id->end_xref_to(); j++)
				{
					Xref * x = (*j).second;
					
					dc.DrawText(wxString::Format(_T("; xref %08x\t %x\n"), x->get_src_addr(), x->get_type()), m_disas_offs, y + hLine - (nrefs) * LINEHEIGHT(hText) - hText);
					nrefs--;
				}
			}
		} else {
			u32 pos = y + hLine - hText;
			dc.SetTextForeground(*wxLIGHT_GREY);
			u8 ch;
			if (!m_trace.readByte(addr, &ch))
				fprintf(stderr, "Can't read address 0x%08x\n",
						addr);
			else {
				dc.DrawText(_T(".db"), m_disas_offs, pos);
				wxString str = wxString::Format(_T("0x%02x"), ch);
				if (isprint(ch))
					str += wxString::Format(_T(" '%c'"), ch);
				dc.DrawText(str, m_disas_offs+50, pos);
			}
		}
		
		y += hLine;
	}

	dc.SetTextForeground(*wxBLACK);
	std::list<Comment *>::iterator ci = comments.begin();
	std::list<wxCoord>::iterator yi = lines.begin();
	for (; ci != comments.end(); ++ci, ++yi) {
		Comment *cmt = (*ci);
		wxRect rect(m_comment_offs + 2, (*yi) + 2,
				GetRect().GetWidth() - m_comment_offs - 2,
				LINEHEIGHT(hText) * cmt->get_lines() - 2);

		dc.SetBrush(wxColour(0xff,0xff,0xaa));
		dc.DrawRectangle(rect.GetLeft() - 2, rect.GetTop() - 2,
				rect.GetWidth() + 2, rect.GetHeight() + 2);
		dc.DrawLabel(_U(cmt->get_comment().c_str()), rect);
	}
	comments.clear();
	lines.clear();
}


void CodeViewCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
}


void CodeViewCanvas::OnScroll(wxScrollWinEvent& event)
{
	Refresh();
	m_current_first_addr = m_gprox->get_addr_for_line(GetFirstVisibleLine());
	m_current_first_addr_set = true;
	//printf("scrolled [first addr %x]\n", m_current_first_addr);
	event.Skip();
}

void CodeViewCanvas::OnIdle(wxIdleEvent&)
{
}

void CodeViewCanvas::OnFont(wxCommandEvent & WXUNUSED(event))
{
	G_font = wxGetFontFromUser(this, G_font);
	wxPaintDC dc((wxWindow *)this);
	dc.SetFont(G_font);
	G_font_height = dc.GetCharHeight();
	Refresh();
}

wxCoord CodeViewCanvas::OnGetLineHeight(size_t n) const
{
	if (m_gprox->get_line_count() == 0)
		return 0;
	
	address_t addr = m_gprox->get_addr_for_line(n);
	MemlocData * id = m_trace.lookup_memloc(addr);

	u32 ch = G_font_height; 

	u32 src_xref_count = 0;
	
	if (id)
		src_xref_count = id->count_xrefs_to();
	
	return (1 + src_xref_count) * LINEHEIGHT(ch);
}



bool CodeViewCanvas::getSelection(address_t * addr)
{
	if (addr)
		*addr = m_sel_addr;
	return m_is_sel;
}

CodeViewCanvas::CodeViewCanvas(DocumentWindow * parent, Document &doc)
: wxVScrolledWindow(parent, wxID_ANY), m_doc(doc), m_trace(*doc.getTrace())
{
	if (!G_font.Ok())
	{
		wxPaintDC dc(this);
		G_font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T(""));
		dc.SetFont(G_font);
		G_font_height = dc.GetCharHeight();
	}
	
	m_parent = parent;

	m_gprox = new GuiProxy(&m_trace, parent);
	
	SetLineCount(m_gprox->get_line_count());
	m_disas_offs = 250;
	m_sym_offs = 100;
	m_comment_offs = 400;
	m_is_sel = 0;
	m_sel_addr = 0;
	m_current_first_addr = 0;
	m_current_first_addr_set = false;
	ScrollToLine(0);
	
	
}

BEGIN_EVENT_TABLE(CodeViewCanvas, wxVScrolledWindow)
EVT_IDLE(CodeViewCanvas::OnIdle)
EVT_PAINT(CodeViewCanvas::OnPaint)
EVT_SCROLLWIN(CodeViewCanvas::OnScroll)
EVT_KEY_DOWN(CodeViewCanvas::OnKeyDown)
EVT_LEFT_DOWN(CodeViewCanvas::OnMouseDown)
EVT_RIGHT_DOWN(CodeViewCanvas::OnRightDown)
EVT_MENU(ID_GotoBranch, CodeViewCanvas::OnGotoBranch)
EVT_MENU(ID_ChangeSymbol, CodeViewCanvas::OnSymbolChange)
EVT_MENU(ID_Analyze, CodeViewCanvas::OnAnalyze)
EVT_MENU(ID_GotoAddress, CodeViewCanvas::OnGoto)
EVT_MENU(ID_GotoXREF, CodeViewCanvas::OnXREFs)
END_EVENT_TABLE()
