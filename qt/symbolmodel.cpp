#include <QColor>
#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include "document.h"
#include "xref.h"
#include "memlocdata.h"
#include "program_flow_analysis.h"

#include "symbolmodel.h"


QTSymbolModel::QTSymbolModel(QTRuntimeModel *model)
 : QAbstractItemModel(NULL), m_model(model)
{ }

QTSymbolModel::~QTSymbolModel()
{ }

int QTSymbolModel::rowCount(const QModelIndex &parent) const
{
	return m_model->getTrace().get_symbol_count();
}

int QTSymbolModel::columnCount(const QModelIndex & parent) const
{
	return 1;
}

QModelIndex QTSymbolModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

QModelIndex QTSymbolModel::index(int row, int column,
		const QModelIndex &parent) const
{
	printf("index %dx%d\n", row, column);
	return createIndex(row, column);
}

QVariant QTSymbolModel::data(const QModelIndex &index, int role) const
{
	printf("data %dx%d\n", index.row(), index.column());
	switch (role) {
	case Qt::BackgroundRole:
		if (index.row() & 1)
			return QVariant(QColor(245,245,245));
		else
			return QVariant(QColor(255,255,255));
		break;
	case Qt::DisplayRole: {
		const Symbol *sym = m_model->getTrace().find_ordered_symbol(
				index.row(), SymbolList::SYMORDER_NAME);
		if (!sym) return QVariant();
		return QVariant(QString(sym->get_name().c_str()));
		} break;
	default:
		return QVariant();
	}

	return QVariant();
}

address_t QTSymbolModel::getAddress(int row)
{
	const Symbol *sym = m_model->getTrace().find_ordered_symbol(
			row, SymbolList::SYMORDER_NAME);
	return sym->get_addr();
}

void QTSymbolModel::flush()
{
	reset();
}
