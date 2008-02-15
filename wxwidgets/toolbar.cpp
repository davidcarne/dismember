#include "documentwindow.h"
#include "ids.h"
#include "toolbar.h"

#include "../icons/icon_new.xpm"
#include "../icons/icon_open.xpm"
#include "../icons/icon_save.xpm"
#include "../icons/icon_close.xpm"
#include "../icons/icon_font.xpm"
#include "../icons/icon_goto.xpm"
#include "../icons/icon_rename.xpm"
#include "../icons/icon_binary.xpm"

TopToolBar::TopToolBar(wxFrame *frame)
 : wxToolBar(frame, wxID_ANY, wxDefaultPosition, wxSize(600, 25), wxTB_HORIZONTAL)
{
	Build();
}

void TopToolBar::Build()
{
	wxBitmap ico_new(icon_new);
	wxBitmap ico_open(icon_open);
	wxBitmap ico_save(icon_save);
	wxBitmap ico_close(icon_close);
	wxBitmap ico_font(icon_font);
	wxBitmap ico_goto(icon_goto);
	//wxBitmap ico_rename(icon_rename);
	wxBitmap ico_binary(icon_binary);

	AddTool(ID_New, ico_new, _T("New..."));
	AddTool(ID_Open, ico_open, _T("Open..."));
	AddTool(ID_Save, ico_save, _T("Save..."));
	AddTool(ID_Close, ico_close, _T("Quit"));
	AddSeparator();
	AddTool(ID_Font, ico_font, _T("Font..."));
	AddTool(ID_GotoAddress, ico_goto, _T("Goto Adress..."));
	//AddTool(ID_ChangeSymbol, ico_rename, _T("Change Symbol..."));
	AddTool(ID_LoadAuto, ico_binary, _T("Autoload..."));

	Realize();
}


