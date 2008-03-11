
/**
 * Wxwidgets application for the armtrace gui
 */
class ArmTraceWXApp : public wxApp
{
	DECLARE_EVENT_TABLE()
	
	/** Event handling opening a new document */
	void OnOpen(wxCommandEvent& event);	
	
	/** Event handling creating a new document */
	void OnNew(wxCommandEvent & event);
	
public:
	/** Event handling initialization */
	virtual bool OnInit();
	
};





