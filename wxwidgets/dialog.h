#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <wx/wx.h>
#include "guidialog.h"
#include <map>
#include "wxguiutils.h"

class wxguiWidget : public guiWidget
{
 public:
	wxguiWidget(void *widget, int (* get)(void *widget));
	wxguiWidget(void *widget, bool (* get)(void *widget));
	wxguiWidget(void *widget, std::string (* get)(void *widget));
	~wxguiWidget();

	std::string text();
	int number();
	bool boolean();

 protected:
 	void *widget;
	int (* get_integer)(void *widget);
	std::string (* get_text)(void *widget);
	bool (* get_boolean)(void *widget);
};


class wxguiDialog : public guiDialog, public wxDialog
{
 public:
 	wxguiDialog(std::string name);
	~wxguiDialog();

	void addTextInput(std::string idx, std::string name,
			std::string def_text);

	void addSubmit(std::string text);

	guiWidget &getValue(std::string idx);

	void showModal();
	void show();

	void OnSubmit(wxCommandEvent &event);
 protected:
	std::string name;
	wxPanel *panel;
	wxBoxSizer *vbox, *hbox;
 	std::map<std::string, wxguiWidget *> map;
};

#endif
