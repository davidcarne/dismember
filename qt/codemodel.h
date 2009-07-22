#ifndef _DOCUMENTPROXYMODEL_H_
#define _DOCUMENTPROXYMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "runtimemodel.h"
#include "guiproxy.h"

class Workspace;

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

 private:
	QString displayText(ProjectModel &, address_t addr) const;
	QString displayXrefs(ProjectModel &, address_t addr) const;
	QString displaySymbol(ProjectModel &, address_t addr) const;
	QString displayComment(ProjectModel &, address_t addr) const;
	QString displayXrefBrief(ProjectModel &, address_t addr) const;

	QTRuntimeModel *m_model;
};

#endif
