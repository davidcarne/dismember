#ifndef _DOCUMENTPROXYMODEL_H_
#define _DOCUMENTPROXYMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "guiproxy.h"

class Document;

class DocumentProxyModel : public QAbstractItemModel
{
 public:
	DocumentProxyModel(Document &doc);
	~DocumentProxyModel();

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

 private:
	QString displayText(address_t addr) const;
	QString displayXrefs(address_t addr) const;
	QString displaySymbol(address_t addr) const;
	QString displayComment(address_t addr) const;

	Document &m_doc;
	GuiProxy *m_gprox;
};

#endif
