#include "codeview.h"
#include "ids.h"
#include "../memlocdata.h"
#include "../datatype.h"
#include <assert.h>
#include "datatypelist.h"
#include "codeviewcanvas.h"

DataTypeListView::DataTypeListView(CodeView *parent, Trace &ctx)  : wxListBox(parent,wxID_ANY, wxDefaultPosition, wxSize(140,-1), 0, wxLB_SORT), m_ctx(ctx), m_parent(parent)
{
	DataTypeReg::datatypereg_ci dt_i = m_ctx.getDataTypeBegin();
	DataTypeReg::datatypereg_ci dt_end = m_ctx.getDataTypeEnd();
	
	printf("starting ins data\n");
	for(; dt_i != dt_end; dt_i ++)
	{
		
		printf("app datatype %s\n", (*dt_i).second->getName().c_str());
		Append(_U((*dt_i).second->getName().c_str()));
	}
}

DataTypeListView::~DataTypeListView()
{
	
}

void DataTypeListView::createSelectedDataType(void)
{
	address_t seladdr;
	if (m_parent->m_canvas->getSelection(&seladdr))
	{
		wxString selname = wxControlWithItems::GetStringSelection();
		
		if (!selname.length())
		{
			return;
		}
		
		DataType * dt = m_ctx.lookupDataType((const char *)selname.fn_str());
		assert(dt);
		m_ctx.createMemlocDataAt(dt, seladdr);
		
		
		m_parent->m_canvas->updateLines();
	}
}

void DataTypeListView::OnDblClick(wxMouseEvent& m)
{	
	createSelectedDataType();
}

BEGIN_EVENT_TABLE(DataTypeListView, wxWindow)
EVT_LEFT_DCLICK(DataTypeListView::OnDblClick)
END_EVENT_TABLE()
