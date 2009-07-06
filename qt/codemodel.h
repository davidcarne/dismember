#ifndef _DOCUMENTPROXYMODEL_H_
#define _DOCUMENTPROXYMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "runtimemodel.h"
#include "guiproxy.h"

class Document;

class QTCodeModel : public QAbstractItemModel
{
 public:
	QTCodeModel(QTRuntimeModel *model);
	~QTCodeModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;
	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index,
			int role = Qt::DisplayRole) const;

	void flush();

	bool isDefined(int row);
	void analyze(int row);
	void undefine(int row);

	void setSymbol(int row, QString str);
	QString getSymbol(int row);

	int getJumpLine(int row);

 private:
	QString displayText(Trace &, address_t addr) const;
	QString displayXrefs(Trace &, address_t addr) const;
	QString displaySymbol(Trace &, address_t addr) const;
	QString displayComment(Trace &, address_t addr) const;
	QString displayXrefBrief(Trace &, address_t addr) const;

	QTRuntimeModel *m_model;
};

#endif
