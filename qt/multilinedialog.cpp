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

#include "multilinedialog.h"

QTMultiLineDialog::QTMultiLineDialog(QWidget *parent)
 : QDialog(parent), m_buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal)
{
	m_layout.addWidget(&m_label,    0, 0);
	m_layout.addWidget(&m_textedit, 1, 0);
	m_layout.addWidget(&m_buttons,  2, 0);
	connect(&m_buttons, SIGNAL(accepted()),
			this, SLOT(accept()));
	connect(&m_buttons, SIGNAL(rejected()),
			this, SLOT(reject()));
	setLayout(&m_layout);
}

QString QTMultiLineDialog::getText(QWidget *parent, const QString &title,
		const QString &label, const QString &text, bool *ok)
{
	QTMultiLineDialog d(parent);
	d.setText(text);
	d.setLabel(label);
	d.setWindowTitle(title);
	d.exec();
	if (ok) {
		if (d.result() == QDialog::Accepted)
			*ok = true;
		else if (d.result() == QDialog::Rejected)
			*ok = false;
	}
	return d.text();
}

QString QTMultiLineDialog::text()
{
	return m_textedit.toPlainText();
}

void QTMultiLineDialog::setText(const QString &text)
{
	m_textedit.setText(text);
}

void QTMultiLineDialog::setLabel(const QString &label)
{
	m_label.setText(label);
}
