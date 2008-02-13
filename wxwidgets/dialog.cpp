#include "dialog.h"
#include <stdexcept>
#include <typeinfo>

wxguiDialog::wxguiDialog(std::string _name)
 : guiDialog(_name), wxDialog(NULL, -1, _U(_name.c_str()), wxDefaultPosition),
   name(_name)
{
	panel = new wxPanel(this, -1);
	vbox = new wxBoxSizer(wxVERTICAL);
	//hbox = new wxBoxSizer(wxHORIZONTAL);

	//vbox->Add(panel, 1);
	//vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

}

wxguiDialog::~wxguiDialog()
{}


static std::string textctrl_gettext(void *widget)
{
	wxTextCtrl *w = (wxTextCtrl *)widget;
	return std::string(w->GetLineText(0).fn_str());
}

void wxguiDialog::addTextInput(std::string idx, std::string name,
			std::string def_text)
{
	wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *text = new wxStaticText(panel, -1, _U(name.c_str()));
	wxTextCtrl *tc = new wxTextCtrl(panel, -1, _U(def_text.c_str()));
	box->Add(text, 1, wxRIGHT);
	box->Add(tc, 1);
	vbox->Add(box, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	vbox->Add(-1, 10);
	map[idx] = new wxguiWidget(tc, textctrl_gettext);
}

enum {
	ID_SUBMIT = 1,
};

void wxguiDialog::OnSubmit(wxCommandEvent &WXUNUSED(event))
{
	Close();
}

void wxguiDialog::addSubmit(std::string name)
{
	wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
	wxButton *bu = new wxButton(panel, ID_SUBMIT, _U(name.c_str()));
	Connect(ID_SUBMIT, wxEVT_COMMAND_BUTTON_CLICKED, 
      		wxCommandEventHandler(wxguiDialog::OnSubmit));
	box->Add(bu, 1);
	vbox->Add(box, 0, wxALIGN_CENTER | wxBOTTOM, 10);
}

void wxguiDialog::show()
{
	panel->SetSizer(vbox);
	panel->Fit();
	SetClientSize(panel->GetSize());
	Centre();
	Show();
}

void wxguiDialog::showModal()
{
	panel->SetSizer(vbox);
	panel->Fit();
	SetClientSize(panel->GetSize());
	Centre();
	ShowModal();
}

guiWidget &wxguiDialog::getValue(std::string idx)
{
	return *map[idx];
}


wxguiWidget::wxguiWidget(void *wid, int (* get)(void *widget))
 : widget(wid), get_integer(get), get_text(0), get_boolean(0)
{ }
wxguiWidget::wxguiWidget(void *wid, bool (* get)(void *widget))
 : widget(wid), get_integer(0), get_text(0), get_boolean(get)
{ }
wxguiWidget::wxguiWidget(void *wid, std::string (* get)(void *widget))
 : widget(wid), get_integer(0), get_text(get), get_boolean(0)
{ }

wxguiWidget::~wxguiWidget()
{ }

int wxguiWidget::number()
{
	if (get_integer)
		return (* get_integer)(widget);
	else throw (std::bad_cast());
}

bool wxguiWidget::boolean()
{
	if (get_boolean)
		return (* get_boolean)(widget);
	else throw (std::bad_cast());
}

std::string wxguiWidget::text()
{
	if (get_text)
		return (* get_text)(widget);
	else throw (std::bad_cast());
}

guiDialog *createGuiDialog(std::string name)
{
	return new wxguiDialog(name);
}
