#ifndef _SYMBOLMODEL_H_
#define _SYMBOLMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "runtimemodel.h"
#include "guiproxy.h"

class Document;

class QTSymbolModel : public QAbstractItemModel
{
 public:
	QTSymbolModel(QTRuntimeModel *model);
	~QTSymbolModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;
	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index,
			int role = Qt::DisplayRole) const;

	void flush();
	address_t getAddress(int row);

 private:
	QTRuntimeModel *m_model;
};

#endif
