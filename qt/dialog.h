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
