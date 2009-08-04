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

#ifndef _RUNQUEUEMONITOR_H_
#define _RUNQUEUEMONITOR_H_

#include <QLabel>

#include "runtimemodel.h"

class QTRunQueueMonitor : public QLabel, public QTRuntimeModelListener
{
	Q_OBJECT
 public:
	QTRunQueueMonitor(QWidget *parent);

	void setRuntimeModel(QTRuntimeModel *rt);
	void runtimeUpdated(QTRuntimeEvent *m);

 public slots:
	void timeout();

 protected:
	bool m_running;
	QTRuntimeModel *m_model;
};

#endif
