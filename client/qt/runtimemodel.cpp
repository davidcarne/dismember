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

#include <QTimer>
#include "runtimemodel.h"
#include "workspace.h"


QTRuntimeEvent::QTRuntimeEvent(QTRuntimeModel *model, QTRuntimeEvent::Type type,
		address_t start, address_t end)
 : m_model(model), m_type(type), m_start(start), m_end(end),
 	m_flags(QTRuntimeEvent::NoFlags)
{ }


QTRuntimeEvent::QTRuntimeEvent(QTRuntimeModel *model, QTRuntimeEvent::Type type,
		address_t start, QTRuntimeEvent::Flags flags)
 : m_model(model), m_type(type), m_start(start), m_flags(flags)
{ }

QTRuntimeEvent::Type QTRuntimeEvent::type()
{
	return m_type;
}

QTRuntimeEvent::Flags QTRuntimeEvent::flags()
{
	return m_flags;
}

QTRuntimeModel *QTRuntimeEvent::model()
{
	return m_model;
}

address_t QTRuntimeEvent::start()
{
	return m_start;
}

address_t QTRuntimeEvent::end()
{
	return m_end;
}

QTRuntimeModel::QTRuntimeModel(Workspace &runtime)
 : m_runtime(runtime), m_gproxy(runtime.getProjectModel()), m_updated(false)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	timer->start(250);
}

QTRuntimeModel::~QTRuntimeModel()
{ }

void QTRuntimeModel::updateTimeout()
{
	if (!m_updated) return;

	std::list<QTRuntimeModelListener *>::iterator it = m_listeners.begin();
	// we'll change this to RuntimeUpdate when we get an update range
	QTRuntimeEvent ev(this, QTRuntimeEvent::RuntimeFlush);
	for (; it != m_listeners.end(); ++it)
		(*it)->runtimeUpdated(&ev);
	m_updated = false;
}

void QTRuntimeModel::postUpdate()
{
	m_updated = true;
}

void QTRuntimeModel::postJump(address_t addr, QTRuntimeEvent::Flags f)
{
	std::list<QTRuntimeModelListener *>::iterator it = m_listeners.begin();
	QTRuntimeEvent ev(this, QTRuntimeEvent::RuntimeJump, addr, f);
	for (; it != m_listeners.end(); ++it)
		(*it)->runtimeUpdated(&ev);
}

void QTRuntimeModel::registerRuntimeModelListener(QTRuntimeModelListener *l)
{
	l->setRuntimeModel(this);
	m_listeners.push_back(l);
}

void QTRuntimeModel::unregisterRuntimeModelListener(QTRuntimeModelListener *l)
{
	m_listeners.remove(l);
}

Workspace &QTRuntimeModel::getRuntime()
{
	return m_runtime;
}

GuiProxy &QTRuntimeModel::getProxy()
{
	return m_gproxy;
}

I_ProjectModel &QTRuntimeModel::getProjectModel()
{
	return *m_runtime.getProjectModel();
}


QTRuntimeModel *QTRuntimeModel::create(Workspace &rt)
{
	return new QTRuntimeModel(rt);
}


QTRuntimeModelListener::~QTRuntimeModelListener() 
{}