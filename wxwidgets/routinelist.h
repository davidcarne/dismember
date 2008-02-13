#ifndef _ROUTINELIST_H_
#define _ROUTINELIST_H_

class RoutineListView: public wxListView
{
public:
	RoutineListView(wxFrame *parent, Trace &ctx);
	void OnSelect(wxListEvent& event);
	void OnShow(wxCommandEvent& event);
	void OnSort(wxListEvent& event);
	void OnRightDown(wxMouseEvent& m);
	virtual wxString OnGetItemText(long item, long column) const;
	inline bool isAllowed(const Symbol *sym) const;
	void Update();
	void UpdateSymbol(Symbol *s, HookChange hc);
	DECLARE_EVENT_TABLE();
private:
	Trace &m_trace;
	wxFrame *m_parent;
	wxMenu m_menu;
	wxMenuItem *m_data, *m_subs, *m_locs;
	std::map<int, int> m_omap;
	
	enum {
		ADDRESS,
		SYMBOL
	} m_sort_by;
};

#endif