#ifndef _CODEVIEW_H_
#define _CODEVIEW_H_
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QShowEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QStack>
#include <QList>

#include "runtimemodel.h"
#include "codemodel.h"

class QTCodeController
{
 public:
	QTCodeController(QTRuntimeModel &model) {};

	virtual void update() = 0;
	virtual void analyze(address_t addr) = 0;
	virtual void undefine(address_t addr) = 0;
	virtual void setComment(address_t addr, const QString &cmt) = 0;
	virtual void setSymbol(address_t addr, const QString &sym) = 0;
	virtual void setDataType(address_t addr, DataType *dt) = 0;
	virtual ~QTCodeController() = 0;
};

class QTCodeView : public QTableView, public QTRuntimeModelListener
{
 public:
	QTCodeView(QWidget *parent = NULL);

	void setRuntimeModel(QTRuntimeModel *model);
	void runtimeUpdated(QTRuntimeEvent *m);

	void showEvent(QShowEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	bool getSelectionAddress(address_t *addr);
	QTCodeController *getController();

 private:
 	void setSymbol(address_t addr);
	void setComment(address_t addr);
	void scrollTo(address_t addr);
	void setCurrentIndex(address_t addr);
	bool hasJump(address_t addr);
	bool isDefined(address_t addr);
	void jump(address_t addr);
	QList<QAction *> *createXrefMenu(address_t addr);

	QTCodeModel *m_model;
	QTCodeController *m_controller;
	QTRuntimeModel *m_runtime;
	QStack<address_t> m_jumpStack;
};

#endif
