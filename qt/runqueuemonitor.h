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
