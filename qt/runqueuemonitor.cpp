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
