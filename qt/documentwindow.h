#ifndef _DOCUMENTWINDOW_H_
#define _DOCUMENTWINDOW_H_

#include <QMainWindow>
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
	virtual ~DocumentWindow();
	virtual void postUpdate();

 private slots:
	void quit();
	void open();
	void save();
	void saveas();
	void updateTimeout();

 private:
	DocumentWindow(Document & doc);
	friend DocumentGui *setupDocumentGui(Document & doc);

	void error(QString brief, QString err);
	bool confirmSave();
	Document &m_doc;
	bool m_fileChanged;
	bool m_updated;
	Ui::MainWindow m_ui;
	QString m_filename;
	DocumentProxyModel *m_model;
};

#endif
