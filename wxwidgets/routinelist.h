#ifndef _ROUTINELIST_H_
#define _ROUTINELIST_H_

/**
 *	View of all defined symbols
 */
class SymbolListView: public wxListView
{
public:
	/**
	 * Create a new symbol list view, sourcing its data from document doc
	 */
	SymbolListView(wxFrame *parent, Document &doc);
	
	/** 
	 * Event triggered on selecting a new item
	 */
	void OnSelect(wxListEvent& event);
	
	/** 
	 * Event triggered on being displayed
	 */
	void OnShow(wxCommandEvent& event);
	
	/** 
	 * Event triggered on the sort order being changed
	 */
	void OnSort(wxListEvent& event);
	
	/** 
	 * Event triggered on right-mouse-click
	 */
	void OnRightDown(wxMouseEvent& m);
	
	/** 
	 * Overload used to get a row/col item text
	 */
	virtual wxString OnGetItemText(long item, long column) const;
	
	/**
	 * Notify the view that its contents have changed, and that it needs to redraw
	 */
	void Update();
	
	/**
	 * Notify the view that a specific symbol has been changed, and to update
	 */
	void UpdateSymbol(Symbol *s, HookChange hc);
	
	/**
	 * ignore me
	 */
	DECLARE_EVENT_TABLE();
	
private:
	/**
	 * Filter to check if the datatype is being displayed currently
	 * @param symbol to check
	 * @return if the view should display that symbol
	 */
	bool isAllowed(const Symbol *sym) const;
	
	Document &m_doc;
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