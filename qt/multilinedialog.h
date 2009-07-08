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
