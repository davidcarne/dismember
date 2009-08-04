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

#ifndef _MULTILINEDIALOG_H_
#define _MULTILINEDIALOG_H_

#include <QDialog>
#include <QString>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>

class QTMultiLineDialog : public QDialog
{
 public:
	QTMultiLineDialog(QWidget *parent);
	static QString getText(QWidget *parent, const QString &title,
			const QString &label, const QString &text = QString(),
			bool *ok = 0);

	QString text();
	
	void setText(const QString &text);
	void setLabel(const QString &label);

 private:
	QTextEdit m_textedit;
	QGridLayout m_layout;
	QLabel m_label;
	QDialogButtonBox m_buttons;
};

#endif
