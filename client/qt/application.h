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

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include "guiglue.h"
#include "guiproxy.h"
#include "application.ui.h"
#include "workspace.h"
#include "runqueuemonitor.h"

class QTApplication : public QMainWindow
{
	Q_OBJECT
 public:
	QTApplication();
	virtual ~QTApplication();
	void closeEvent(QCloseEvent *);

 private slots:
	void quit();
	void open();
	void save();
	void saveas();

 private:
	void error(QString brief, QString err);
	bool confirmSave();
	void replaceRuntime();

	bool m_fileChanged;
	bool m_updated;
	Workspace *m_runtime;
	Ui::MainWindow m_ui;
	QString m_filename;
};

#endif
