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

#ifndef _CODEMODEL_H_
#define _CODEMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "runtimemodel.h"
#include "guiproxy.h"
#include "i_comment.h"

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

	QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;

	void flush();

 private:
	QString displayText(I_ProjectModel &, address_t addr) const;
	QString displayXrefs(I_ProjectModel &, address_t addr) const;
	QString displaySymbol(I_ProjectModel &, address_t addr) const;
	QString displayComment(I_ProjectModel &, address_t addr) const;
	QString displayXrefBrief(I_ProjectModel &, address_t addr) const;

	QTRuntimeModel *m_model;
};

#endif
