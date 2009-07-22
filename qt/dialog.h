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

#ifndef _DIALOG_H_
#define _DIALOG_H_
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include "hash_map.h"
#include "guidialog.h"

class QTDialog : public guiDialog
{
 public:
	QTDialog(std::string name);
	~QTDialog();

	void addTextInput(std::string idx, std::string name, std::string dt);
	void addSubmit(std::string text);

	guiWidget &getValue(std::string idx);

	void show();
	void showModal();
 private:
	int m_row;
	int m_column;
	QDialog *m_dialog;
	QGridLayout *m_layout;
	std::hash_map<std::string, QLineEdit *> m_map;
};

#endif
