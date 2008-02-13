class ArmTraceApp : public wxApp
{
	DECLARE_EVENT_TABLE()
	
	void OnOpen(wxCommandEvent& event);	
	void OnNew(wxCommandEvent & event);
	
public:
	virtual bool OnInit();
	
};





