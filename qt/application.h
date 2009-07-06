#ifndef _DOCUMENTWINDOW_H_
#define _DOCUMENTWINDOW_H_

#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include "guiglue.h"
#include "guiproxy.h"
#include "application.ui.h"
#include "document.h"

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
	Document *m_runtime;
	Ui::MainWindow m_ui;
	QString m_filename;
};

#endif
