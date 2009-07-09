#ifndef _DATATYPEVIEW_H_
#define _DATATYPEVIEW_H_
#include <QString>
#include <QListWidget>
#include <QKeyEvent>
#include <QMouseEvent>

#include "codeview.h"
#include "runtimemodel.h"

class QTDataTypeView : public QListWidget, public QTRuntimeModelListener
{
 public:
	QTDataTypeView(QWidget *parent = NULL);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	void setCodeview(QTCodeView *cv);

 private:
	void createDataType(const QString &str);

	QTRuntimeModel *m_runtime;
	QTCodeView *m_codeview;
};

#endif
