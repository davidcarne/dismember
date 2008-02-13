#include "codeview.h"
#include "ids.h"
#include "codeviewcanvas.h"
#include "routinelist.h"

RoutineList::RoutineList(wxFrame *frame, Trace &ctx)
 : wxListView(frame, wxID_ANY, wxDefaultPosition, wxSize(180, 475), wxLC_REPORT | wxLC_VIRTUAL), m_trace(ctx)
{
	m_parent = frame;
	m_sort_by = ADDRESS;
	m_data = m_menu.AppendCheckItem(wxID_ANY, _T("Show DATA refs"));
	m_locs = m_menu.AppendCheckItem(wxID_ANY, _T("Show LOCS"));
	m_subs = m_menu.AppendCheckItem(wxID_ANY, _T("Show SUBS"));
	m_data->Check(true);
	m_locs->Check(true);
	m_subs->Check(true);

	Callback<Symbol*, RoutineList> *cb = new Callback<Symbol*, RoutineList>;
	cb->setClass(this);
	cb->setCallback(&RoutineList::UpdateSymbol);
	m_trace.registerSymbolHook(cb);
	
	SetWindowStyleFlag(wxLC_SINGLE_SEL | wxLC_REPORT | wxLC_VIRTUAL);
	InsertColumn(0, _U("Symbol"), wxLIST_FORMAT_LEFT, 90);
	InsertColumn(1, _U("Address"), wxLIST_FORMAT_RIGHT, 90);
	InsertColumn(2, _U("Type"), wxLIST_FORMAT_LEFT, 0);
	SetItemCount(0);
}

inline bool RoutineList::isAllowed(const Symbol *sym) const
{
// apparently way too slow
	return true;

	GenericAbstractData *gad;
	if (!m_data->IsChecked() && (gad = sym->get_property("type"))) {
		if (gad->text() == "data")
			return false;
	}
	if (!m_subs->IsChecked() && (gad = sym->get_property("subroutine"))) {
		if (gad->boolean())
			return false;
	}
	//if (!locs->IsChecked())
	return true;
}

void RoutineList::UpdateSymbol(Symbol *s, HookChange hc)
{
	switch (hc) {
	case HOOK_DELETE:
	case HOOK_MODIFY:
		printf("FIXME: Something got updated, fix %s:%d\n",
				__FILE__, __LINE__);
		break;
	case HOOK_CREATE: {
		//Update();
		} break;
	default:
		break;
	}
	
	//Update();
}

void RoutineList::Update()
{
	int n, max = m_trace.get_symbol_count();
#if 0
	SymbolList::symsortorder_e order = SymbolList::SYMORDER_ADDR;
	if (m_sort_by == SYMBOL)
		order = SymbolList::SYMORDER_NAME;
	const Symbol *sym;
	m_omap.clear();
	for (int i = n = 0; i < max; ++i) {
		if (!isAllowed(m_trace.find_ordered_symbol(i, order))) continue;
		m_omap[n++] = i;
	}
	SetItemCount(n);
#else
	SetItemCount(max);
#endif
	Refresh();
}

wxString RoutineList::OnGetItemText(long item, long column) const
{
	SymbolList::symsortorder_e order = SymbolList::SYMORDER_ADDR;
	if (m_sort_by == SYMBOL)
		order = SymbolList::SYMORDER_NAME;
	
#if 0
	item = ((std::map<int, int>)omap)[item]; // const function mapping blows
#endif
	const Symbol *sym = m_trace.find_ordered_symbol(item, order);
	
	if (!sym) throw;
	
	switch (column) {
	case 0: return wxString::Format(_T("%s"), sym->get_name().c_str());
	case 1: return wxString::Format(_T("%08x"), sym->get_addr());
	case 2: {
		GenericAbstractData *gad = sym->get_property("type");
		if (gad)
			return _U(gad->text().c_str());
		else return _U("");
		}
	}
	return _("");
}

void RoutineList::OnSort(wxListEvent& event)
{
	switch (event.GetColumn()) {
	case 0:	m_sort_by = SYMBOL; Update(); break;
	case 1:	m_sort_by = ADDRESS; Update(); break;
	default: break;
	}
	Update();
}

void RoutineList::OnSelect(wxListEvent& WXUNUSED(event))
{
	SymbolList::symsortorder_e order = SymbolList::SYMORDER_ADDR;
	if (m_sort_by == SYMBOL)
		order = SymbolList::SYMORDER_NAME;
	
	
	const Symbol *sym = m_trace.find_ordered_symbol(GetFirstSelected(), order);
	
	if (sym)
		((CodeView *)m_parent)->m_canvas->jumpToAddr(sym->get_addr());
}


void RoutineList::OnShow(wxCommandEvent& event)
{
	Update();
}

void RoutineList::OnRightDown(wxMouseEvent& m)
{
	PopupMenu(&m_menu, m.GetPosition());
	m.Skip();
}

BEGIN_EVENT_TABLE(RoutineList, wxListView)
EVT_RIGHT_DOWN(RoutineList::OnRightDown)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, RoutineList::OnSelect)
EVT_LIST_COL_CLICK(wxID_ANY, RoutineList::OnSort)
EVT_MENU(wxID_ANY, RoutineList::OnShow)
END_EVENT_TABLE()
