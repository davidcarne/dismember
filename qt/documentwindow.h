#ifndef _DOCUMENTWINDOW_H_
#define _DOCUMENTWINDOW_H_

#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include "guiglue.h"
#include "guiproxy.h"
#include "documentwindow.ui.h"
#include "documentproxymodel.h"

class Document;

class DocumentWindow : public QMainWindow, public DocumentGui
{
	Q_OBJECT
 public:
	DocumentWindow();
	virtual ~DocumentWindow();
	virtual void postUpdate();
	void closeEvent(QCloseEvent *);

 private slots:
	void quit();
	void open();
	void save();
	void saveas();
	void updateTimeout();

 private:
	void error(QString brief, QString err);
	bool confirmSave();
	void replaceDocument();

	bool m_fileChanged;
	bool m_updated;
	Document *m_doc;
	Ui::MainWindow m_ui;
	QString m_filename;
	DocumentProxyModel *m_model;
};

#endif
