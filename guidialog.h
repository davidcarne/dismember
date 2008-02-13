#ifndef _GUIDIALOG_H_
#define _GUIDIALOG_H_
#include <string>
#include "abstractdata.h"

typedef AbstractData guiWidget;

class guiDialog
{
 public:
	guiDialog(std::string name) {};
	virtual ~guiDialog() {};

	virtual void addTextInput(std::string idx, std::string name,
			std::string def_text) = 0;
	virtual void addSubmit(std::string text) = 0;


	virtual guiWidget &getValue(std::string idx) = 0;

	virtual void show() = 0;
	virtual void showModal() = 0;
};

guiDialog *createGuiDialog(std::string name);

#endif
