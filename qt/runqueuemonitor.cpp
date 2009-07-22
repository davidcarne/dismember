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

#include <QLabel>
#include <QTimer>

#include "document.h"
#include "runtimemodel.h"
#include "runqueuemonitor.h"
#include "run_queue.h"

QTRunQueueMonitor::QTRunQueueMonitor(QWidget *parent)
 : QLabel(parent), m_running(false)
{
	setText(QString("RQ: idle"));

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
	timer->start(250);
}

void QTRunQueueMonitor::timeout()
{
	bool running = m_model->getRuntime().getRunQueue()->isRunning();
	if (running != m_running) {
		if (!running)
			setText(QString("RQ: idle"));
		else {
			QString str = QString(m_model->getRuntime().getRunQueue()->currentTaskName().c_str());
			setText(QString("RQ: running ").append(str));
		}
		m_running = running;
	}
}

void QTRunQueueMonitor::setRuntimeModel(QTRuntimeModel *rt)
{
	m_model = rt;
	m_running = false;
	timeout();
}

void QTRunQueueMonitor::runtimeUpdated(QTRuntimeEvent *m)
{
	timeout();
}
