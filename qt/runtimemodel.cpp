#include <QTimer>
#include "runtimemodel.h"
#include "document.h"


QTRuntimeEvent::QTRuntimeEvent(QTRuntimeModel *model, QTRuntimeEvent::Type type,
		address_t start, address_t end)
 : m_model(model), m_type(type), m_start(start), m_end(end),
 	m_flags(QTRuntimeEvent::NoFlags)
{ }


QTRuntimeEvent::QTRuntimeEvent(QTRuntimeModel *model, QTRuntimeEvent::Type type,
		address_t start, QTRuntimeEvent::Flags flags)
 : m_model(model), m_type(type), m_start(start), m_end(0), m_flags(flags)
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

QTRuntimeModel::QTRuntimeModel(Document &runtime)
 : m_runtime(runtime), m_gproxy(runtime.getTrace()), m_updated(false)
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

Document &QTRuntimeModel::getRuntime()
{
	return m_runtime;
}

GuiProxy &QTRuntimeModel::getProxy()
{
	return m_gproxy;
}

Trace &QTRuntimeModel::getTrace()
{
	return *m_runtime.getTrace();
}


QTRuntimeModel *QTRuntimeModel::create(Document &rt)
{
	return new QTRuntimeModel(rt);
}
