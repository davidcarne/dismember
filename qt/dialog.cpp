#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "dialog.h"

QTDialog::QTDialog(std::string name)
 : guiDialog(name), m_row(0), m_column(0)
{
	m_dialog = new QDialog(NULL);
	m_layout = new QGridLayout();
	m_layout->setColumnStretch(1,1);
	m_dialog->setWindowTitle(QString(name.c_str()));
}

QTDialog::~QTDialog()
{
	delete m_dialog;
	delete m_layout;
}

void QTDialog::addTextInput(std::string idx, std::string name, std::string dt)
{
	int frameStyle = QFrame::Sunken | QFrame::Panel;
	QLabel *label = new QLabel(QString(name.c_str()));
	QLineEdit *line = new QLineEdit(QString(dt.c_str()));
	if (m_column & 1) {
		m_column = 0;
		m_row++;
	}
	m_layout->addWidget(label, m_row, 0);
	m_layout->addWidget(line,  m_row, 1);
	++m_row;
	m_map[idx] = line;
}

void QTDialog::addSubmit(std::string text)
{
	QPushButton *button = new QPushButton(QString(text.c_str()));
	QObject::connect(button, SIGNAL(clicked()), m_dialog, SLOT(accept()));
	if (m_column & 1) {
		m_column = 0;
		m_row++;
	}
	m_layout->addWidget(button, m_row, m_column);
	++m_column;
}

guiWidget &QTDialog::getValue(std::string idx)
{
	// fixme: memory leak
	return *(new AbstractData(m_map[idx]->text().toStdString()));
}

void QTDialog::show()
{
	m_dialog->setLayout(m_layout);
	m_dialog->show();
	m_dialog->raise();
	m_dialog->activateWindow();
}

void QTDialog::showModal()
{
	m_dialog->setLayout(m_layout);
	m_dialog->exec();
}

guiDialog *createGuiDialog(std::string name)
{
	return new QTDialog(name);
}
