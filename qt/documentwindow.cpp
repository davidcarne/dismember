#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include "../loaders/loaderfactory.h"
#include "documentwindow.h"
#include "documentproxymodel.h"
#include "document.h"
#include "dummygui.h"

DocumentWindow::DocumentWindow()
 : QMainWindow(NULL), m_fileChanged(false), m_updated(false)
{
	m_ui.setupUi(this);
	connect(m_ui.a_quit, SIGNAL(activated()), this, SLOT(quit()));
	connect(m_ui.a_open, SIGNAL(activated()), this, SLOT(open()));
	connect(m_ui.a_save, SIGNAL(activated()), this, SLOT(save()));
	connect(m_ui.a_saveas, SIGNAL(activated()), this, SLOT(saveas()));
	connect(this, SIGNAL(destroyed()), this, SLOT(quit()));

	m_doc = NULL;
	m_model = NULL;
	replaceDocument();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	timer->start(250);
}

DocumentWindow::~DocumentWindow()
{ }

void DocumentWindow::open()
{
	if (!confirmSave())
		return;
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File.."), QString(),
			tr("Binary Files (*.bin *.hex *.o);;"
			   "Dismember Compressed Files (*.dcf);;"
			   "All Files (*.*)"));
	if (fileName == QString()) // cancelled
		return;

	if (fileName.endsWith(".dcf", Qt::CaseInsensitive)) {
		m_filename = fileName;
		// todo: load dcf
	} else {
		FILE *fp = fopen(fileName.toStdString().c_str(), "r");
		if (!fp) {
			const char *serr = strerror(errno);
			error(tr("Open failed"),
				fileName
				.append(": ")
				.append(serr));
			return;
		}
		replaceDocument();
		if (!FileLoaderMaker::autoLoadFromFile(fp, m_doc->getTrace())) {
			error(tr("Open failed"),
				fileName
				.append(": ")
				.append(tr("File load failed!")));
		}
		fclose(fp);
	}
	m_fileChanged = false;
	postUpdate();
}

void DocumentWindow::closeEvent(QCloseEvent *event)
{
	if (confirmSave())
		event->accept();
	else
		event->ignore();
}

void DocumentWindow::replaceDocument()
{
	Document *n_doc = new Document();
	DocumentProxyModel *n_model = new DocumentProxyModel(n_doc);
	m_ui.e_assembly->setModel(n_model);
	if (m_model) delete m_model;
	if (m_doc) delete m_doc;
	m_doc = n_doc;
	m_model = n_model;
	DummyGUI *gui = (DummyGUI *)m_doc->getDocumentGui();
	gui->hookUpdate(this);
}

void DocumentWindow::save()
{
	if (m_filename == QString()) {
		saveas(); // calls us back
		return;
	}
	m_doc->saveTo(m_filename.toStdString());
	m_fileChanged = false;
}

void DocumentWindow::saveas()
{
	QString fileName = QFileDialog::getSaveFileName(this,
			tr("Save File..."), QString(),
			tr("Dismember Compressed Files (*.dcf)"));
	if (fileName != QString()) {
		if (fileName.endsWith(".dcf", Qt::CaseInsensitive)) 
			m_filename = fileName;
		else
			m_filename = fileName.append(".dcf");
		save();
	}
}

void DocumentWindow::quit()
{
	if (confirmSave()) {
		// todo: cleanup?
		exit(0);
	}
}

bool DocumentWindow::confirmSave()
{
	if (m_fileChanged) {
		QMessageBox msgBox;
		msgBox.setText("The document has been modified.");
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		switch (msgBox.exec()) {
		case QMessageBox::Save:
			save();
			return true;
		case QMessageBox::Discard:
			return true;
		case QMessageBox::Cancel:
			return false;
		}
	}
	return true;
}

void DocumentWindow::error(QString brief, QString err)
{
	QErrorMessage *em = new QErrorMessage(this);
	em->showMessage(err);
	em->exec();
}

void DocumentWindow::updateTimeout()
{
	if (m_updated) {
		m_model->flush();
		m_updated = false;
	}
}

void DocumentWindow::postUpdate()
{
	//m_fileChanged = true;
	m_updated = true;
}
