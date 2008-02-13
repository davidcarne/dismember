#ifndef _WX_DATATYPELIST_H_
#define _WX_DATATYPELIST_H_
class DataTypeList: public wxListBox
{
public:
	DataTypeList(CodeView *parent, Trace &ctx);
	virtual ~DataTypeList();

	void createSelectedDataType(void);

	DECLARE_EVENT_TABLE();
	
private:
		
		void OnDblClick(wxMouseEvent& m);
		Trace &m_ctx;
		CodeView * m_parent;
};

#endif