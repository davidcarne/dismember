#ifndef _WX_DATATYPELIST_H_
#define _WX_DATATYPELIST_H_
class DataTypeListView: public wxListBox
{
public:
	DataTypeListView(DocumentWindow *parent, Document &ctx);
	virtual ~DataTypeListView();

	void createSelectedDataType(void);
	
	virtual void Update();
	
	DECLARE_EVENT_TABLE();
private:
		
		void OnDblClick(wxMouseEvent& m);
		Document &m_doc;
		Trace &m_ctx;
		DocumentWindow * m_parent;
};

#endif