#ifndef _RUNTIMEMODEL_H_
#define _RUNTIMEMODEL_H_

#include <QObject>
#include "guiglue.h"
#include "guiproxy.h"

#include <list>

class QTRuntimeModel;
class Document;

class QTRuntimeEvent
{
 public:
	enum Type {
		RuntimeUpdate, // range
		RuntimeFlush,  // everything
		RuntimeJump    // one addr
		// room for expansion when DocumentGui gets fleshed out
	};

	QTRuntimeEvent(QTRuntimeModel *model, Type type,
			address_t start = 0, address_t end = 0);

	Type type();
	QTRuntimeModel *model();

	address_t start();
	address_t end();

 private:
	QTRuntimeModel *m_model;
	Type m_type;
	address_t m_start;
	address_t m_end;
};

class QTRuntimeModelListener
{
 public:
	virtual void setRuntimeModel(QTRuntimeModel *rt) = 0;
	virtual void runtimeUpdated(QTRuntimeEvent *m) = 0;
};

class QTRuntimeModel : public QObject, public DocumentGui
{
	Q_OBJECT
 public:
 	static QTRuntimeModel *create(Document &rt);
	~QTRuntimeModel();

	void registerRuntimeModelListener(QTRuntimeModelListener *listener);
	void unregisterRuntimeModelListener(QTRuntimeModelListener *listener);

	Document &getRuntime();
	GuiProxy &getProxy();
	Trace &getTrace(); // convienience

	void postUpdate();

	void postJump(address_t addr);

 public slots:
	void updateTimeout();

 private:
	QTRuntimeModel(Document &runtime);

	Document &m_runtime;
	GuiProxy m_gproxy;
	bool m_updated;
 	std::list<QTRuntimeModelListener *> m_listeners;
};

#endif
