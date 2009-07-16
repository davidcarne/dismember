#ifndef _SEARCHWIDGET_H_
#define _SEARCHWIDGET_H_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include "search.h"
#include "runtimemodel.h"

#include <queue>

class QTFlatButton : public QPushButton
{
 public:
	QTFlatButton(const QIcon &ic, const QString &name, QWidget *parent = 0);

 protected:
	void enterEvent(QEvent *e);
	void leaveEvent(QEvent *e);
};



class QTSearchWidget : public QWidget, public QTRuntimeModelListener
{
	Q_OBJECT
 public:
	QTSearchWidget(QWidget *parent = 0);

	void setRuntimeModel(QTRuntimeModel *rt);
	void runtimeUpdated(QTRuntimeEvent *m);

 public slots:
	void startSearch();
	void stopSearch();
	void nextResult();
	void prevResult();

	void timeout();
	void invalidate(const QString &);

 private:
	int translate();
	void jump();

	SearchResults *m_currentSearch;
	uint8_t m_bytes[256];
	int m_lastcount;
	bool m_searching;
	std::list<address_t>::const_iterator m_iterator;
	QLineEdit m_searchBox;
	QTFlatButton m_prevButton;
	QTFlatButton m_nextButton;
	QLabel m_wrapLabel;
	QTFlatButton m_closeButton;
	QTimer *m_timer;
	QTRuntimeModel *m_runtime;
	std::queue<SearchResults *> m_searchQueue;
};

#endif
