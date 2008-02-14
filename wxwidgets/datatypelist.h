#ifndef _WX_DATATYPELIST_H_
#define _WX_DATATYPELIST_H_
class DataTypeListView: public wxListBox
{
public:
	DataTypeListView(CodeView *parent, Trace &ctx);
	virtual ~DataTypeListView();

	void createSelectedDataType(void);

	DECLARE_EVENT_TABLE();
	
private:
		
		void OnDblClick(wxMouseEvent& m);
		Trace &m_ctx;
		CodeView * m_parent;
};

#endif