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

#ifndef _RUNTIMEMODEL_H_
#define _RUNTIMEMODEL_H_

#include <QObject>
#include "guiglue.h"
#include "guiproxy.h"

#include <list>

class QTRuntimeModel;
class Workspace;

class QTRuntimeEvent
{
 public:
	enum Type {
		RuntimeUpdate, // range
		RuntimeFlush,  // everything
		RuntimeJump    // one addr
		// room for expansion when WorkspaceGui gets fleshed out
	};
	enum Flags {
		NoFlags = 0,
		Select = (1 << 0)
	};

	QTRuntimeEvent(QTRuntimeModel *model, Type type,
			address_t start = address_t(),
			address_t end = address_t());
	QTRuntimeEvent(QTRuntimeModel *model, Type type,
			address_t addr, Flags flags = NoFlags);

	
	Type type();
	Flags flags();
	QTRuntimeModel *model();

	address_t start();
	address_t end();

 private:
	QTRuntimeModel *m_model;
	Type m_type;
	address_t m_start;
	address_t m_end;
	Flags m_flags;
};

class QTRuntimeModelListener
{
 public:
	virtual void setRuntimeModel(QTRuntimeModel *rt) = 0;
	virtual void runtimeUpdated(QTRuntimeEvent *m) = 0;
	virtual ~QTRuntimeModelListener() = 0;
};

class QTRuntimeModel : public QObject, public WorkspaceGui
{
	Q_OBJECT
 public:
 	static QTRuntimeModel *create(Workspace &rt);
	~QTRuntimeModel();

	void registerRuntimeModelListener(QTRuntimeModelListener *listener);
	void unregisterRuntimeModelListener(QTRuntimeModelListener *listener);

	Workspace &getRuntime();
	GuiProxy &getProxy();
	I_ProjectModel &getI_ProjectModel(); // convienience

	void postUpdate();

	void postJump(address_t addr,
			QTRuntimeEvent::Flags f = QTRuntimeEvent::NoFlags);

 public slots:
	void updateTimeout();

 private:
	QTRuntimeModel(Workspace &runtime);

	Workspace &m_runtime;
	GuiProxy m_gproxy;
	bool m_updated;
 	std::list<QTRuntimeModelListener *> m_listeners;
};

#endif
