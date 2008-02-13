#ifndef _CANVAS_H_
#define _CANVAS_H_

class CodeViewCanvas: public wxVScrolledWindow
{
public:
   	CodeViewCanvas(CodeView *parent, Trace & ctx);
	
	CodeView *m_parent;
	void OnPaint(wxPaintEvent& event);
	void OnScroll(wxScrollWinEvent& event);
	void OnSize(wxSizeEvent& event);
	
	virtual wxCoord OnGetLineHeight(size_t n) const;
	
	void OnIdle(wxIdleEvent&);
	void OnMouseDown(wxMouseEvent& m);
	void OnRightDown(wxMouseEvent& m);
	void OnKeyDown(wxKeyEvent& k);
	
	void OnGoto(wxCommandEvent&);
	void OnGotoBranch(wxCommandEvent&);
	void OnSymbolChange(wxCommandEvent&);
	void OnComment(wxCommandEvent&);
	void OnAnalyze(wxCommandEvent&);
	void OnUndefine(wxCommandEvent&);
	void OnXREFs(wxCommandEvent &);
	void OnFont(wxCommandEvent &);
	
	void OnCreateData(bool directFromBox);
	
	void jumpToAddr(address_t addr);
	
	
	bool getSelection(address_t * addr);
	
	void updateLines();
	
	DECLARE_EVENT_TABLE()
private:
		Trace &trace;
	u32 m_disas_offs;
	u32 m_sym_offs;
	u32 m_comment_offs;
	
	GuiProxy * m_gprox;
	
	std::stack<address_t> m_addr_stack;
	bool m_is_sel;
	address_t m_sel_addr;
	address_t m_current_first_addr;
	bool m_current_first_addr_set;
	
};


#endif /* _CANVAS_H_ */
