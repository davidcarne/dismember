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
