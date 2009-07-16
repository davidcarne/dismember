#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include "loaders/loaderfactory.h"
#include "application.h"
#include "runtimemodel.h"
#include "document.h"

QTApplication::QTApplication()
 : QMainWindow(NULL), m_fileChanged(false), m_updated(false)
{
	m_ui.setupUi(this);
	connect(m_ui.a_quit, SIGNAL(activated()), this, SLOT(quit()));
	connect(m_ui.a_open, SIGNAL(activated()), this, SLOT(open()));
	connect(m_ui.a_save, SIGNAL(activated()), this, SLOT(save()));
	connect(m_ui.a_saveas, SIGNAL(activated()), this, SLOT(saveas()));
	connect(m_ui.a_find, SIGNAL(activated()),
			m_ui.e_searchwidget, SLOT(show()));
	connect(m_ui.a_find, SIGNAL(activated()),
			m_ui.e_searchwidget, SLOT(setFocus()));
	connect(m_ui.a_findnext, SIGNAL(activated()),
			m_ui.e_searchwidget, SLOT(nextResult()));
	connect(m_ui.a_findprev, SIGNAL(activated()),
			m_ui.e_searchwidget, SLOT(prevResult()));

	m_runtime = NULL;
	replaceRuntime();
}

QTApplication::~QTApplication()
{ }

void QTApplication::open()
{
	if (!confirmSave())
		return;
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File.."), QString(),
			tr("Binary Files (*.bin *.hex *.s19);;"
			   "Object Files (*.o *.so);;"
			   "Dismember Compressed Files (*.dcf);;"
			   "All Files (*)"));
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
		replaceRuntime();
		if (!FileLoaderMaker::autoLoadFromFile(fp,
				m_runtime->getTrace())) {
			error(tr("Open failed"),
				fileName
				.append(": ")
				.append(tr("File load failed!")));
		}
		fclose(fp);
	}
	m_fileChanged = false;
	m_runtime->postGuiUpdate();
}

void QTApplication::closeEvent(QCloseEvent *event)
{
	if (confirmSave())
		event->accept();
	else
		event->ignore();
}

void QTApplication::replaceRuntime()
{
	Document *n_runtime = new Document();
	QTRuntimeModel *n_model = (QTRuntimeModel *)n_runtime->getDocumentGui();
	n_model->registerRuntimeModelListener(m_ui.e_assembly);
	n_model->registerRuntimeModelListener(m_ui.e_dataview);
	n_model->registerRuntimeModelListener(m_ui.e_symbolview);
	n_model->registerRuntimeModelListener(m_ui.e_datatypeview);
	n_model->registerRuntimeModelListener(m_ui.e_terminalview);
	n_model->registerRuntimeModelListener(m_ui.e_monitor);
	n_model->registerRuntimeModelListener(m_ui.e_searchwidget);
	m_ui.e_datatypeview->setCodeview(m_ui.e_assembly);
	m_ui.e_searchwidget->hide();
	if (m_runtime) delete m_runtime;
	m_runtime = n_runtime;
}

void QTApplication::save()
{
	if (m_filename == QString()) {
		saveas(); // calls us back
		return;
	}
	m_runtime->saveTo(m_filename.toStdString());
	m_fileChanged = false;
}

void QTApplication::saveas()
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

void QTApplication::quit()
{
	if (confirmSave()) {
		// todo: cleanup?
		exit(0);
	}
}

bool QTApplication::confirmSave()
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

void QTApplication::error(QString brief, QString err)
{
	QErrorMessage *em = new QErrorMessage(this);
	em->showMessage(err);
	em->exec();
}
