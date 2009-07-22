/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
	return m_model->getProjectModel().get_symbol_count();
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
	return createIndex(row, column);
}

QVariant QTSymbolModel::data(const QModelIndex &index, int role) const
{
	switch (role) {
	case Qt::BackgroundRole:
		if (index.row() & 1)
			return QVariant(QColor(245,245,245));
		else
			return QVariant(QColor(255,255,255));
		break;
	case Qt::DisplayRole: {
		const Symbol *sym = m_model->getProjectModel().find_ordered_symbol(
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
	const Symbol *sym = m_model->getProjectModel().find_ordered_symbol(
			row, SymbolList::SYMORDER_NAME);
	return sym->get_addr();
}

void QTSymbolModel::flush()
{
	reset();
}
