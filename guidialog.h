/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
