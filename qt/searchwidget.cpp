#include <QLayout>
#include <QSizePolicy>
#include <ctype.h>
#include <unistd.h>
#include "searchwidget.h"

#define IC(x) QIcon(QPixmap(QString(x)))

QTSearchWidget::QTSearchWidget(QWidget *parent)
 : QWidget(parent), m_currentSearch(0), m_lastcount(0), m_searching(false),
   m_closeButton(IC(":/icons/action_close"), QString()),
   m_prevButton(IC(":/icons/search_prev"), QString("Previous")),
   m_nextButton(IC(":/icons/search_next"), QString("Next")),
   m_runtime(0)
{
	m_wrapLabel.setPixmap(QPixmap(QString(":/icons/search_wrap")));

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(&m_closeButton);
	layout->addWidget(&m_searchBox);
	layout->addWidget(&m_prevButton);
	layout->addWidget(&m_nextButton);
	layout->addWidget(&m_wrapLabel);
	layout->setContentsMargins(0,0,0,0);
	m_closeButton.setContentsMargins(0,0,0,0);

	setFocusProxy(&m_searchBox);

	m_wrapLabel.hide();

	connect(&m_searchBox, SIGNAL(returnPressed()),
			this, SLOT(startSearch()));
	connect(&m_searchBox, SIGNAL(textChanged(const QString &)),
			this, SLOT(invalidate(const QString &)));
	connect(&m_prevButton, SIGNAL(clicked()), this, SLOT(prevResult()));
	connect(&m_nextButton, SIGNAL(clicked()), this, SLOT(nextResult()));
	connect(&m_closeButton, SIGNAL(clicked()), this, SLOT(hide()));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
	invalidate(QString());
}

void QTSearchWidget::setRuntimeModel(QTRuntimeModel *rt)
{
	m_runtime = rt;
	m_searchBox.clear();
}

void QTSearchWidget::runtimeUpdated(QTRuntimeEvent *m)
{ }

static bool string_isnum(const char *p)
{
	while (*p && isdigit(*p)) ++p;
	return (*p == 0);
}

static bool string_isxnum(const char *p)
{
	while (*p && isxdigit(*p)) ++p;
	return (*p == 0);
}

int QTSearchWidget::translate()
{
	QString text = m_searchBox.text();
	if (text.isEmpty())
		return 0;
	text.truncate(256);
	std::string s_str = text.toStdString();
	const char *c_str = s_str.c_str();
	if (string_isnum(c_str)) {
		int pos = 256;
		bool ok;
		long long n = text.toLongLong(&ok);
		while (ok && n > 0) {
			uint8_t byte = n % 256;
			n /= 256;
			m_bytes[--pos] = byte;
		}
		if (ok)
			return 256 - pos;
	}
	if (string_isxnum(c_str) ||
			(text.startsWith("0x") && string_isxnum(c_str + 2))) {
		int pos = 256;
		bool ok;
		long long n = text.toLongLong(&ok, 16);
		while (ok && n > 0) {
			uint8_t byte = n % 256;
			n /= 256;
			m_bytes[--pos] = byte;
		}
		if (ok)
			return 256 - pos;
	}
	
	{
		int pos = 256 - strlen(c_str);
		const char *p = c_str;
		while (*p)
			m_bytes[pos++] = *p++;
		return strlen(c_str);
	}

	return 0;
}

void QTSearchWidget::startSearch()
{
	if (m_currentSearch)
		stopSearch();

	int len = translate();
	if (!len)
		return;
	m_currentSearch = Search::submitSearchJob(&m_runtime->getRuntime(),
			m_bytes + 256 - len, len);

	m_prevButton.setEnabled(false);
	m_nextButton.setEnabled(false);
	m_wrapLabel.hide();
	m_lastcount = 0;
	m_searching = true;
	m_timer->start(250);
}

void QTSearchWidget::stopSearch()
{
	if (m_currentSearch) {
		if (!m_currentSearch->jobFinished())
			m_searchQueue.push(m_currentSearch);
		else
			delete m_currentSearch;
		m_currentSearch = 0;
		m_iterator = std::list<address_t>::const_iterator();
	}
	m_timer->stop();
}

void QTSearchWidget::jump()
{
	address_t addr = (*m_iterator);
	if (addr.isValid()) {
		m_runtime->postJump(addr, QTRuntimeEvent::Select);
	}
}

void QTSearchWidget::prevResult()
{
	if (!m_currentSearch) {
		startSearch();
		return;
	}
	bool wrap = false;

	m_searching = false;
	const std::list<address_t> &res = m_currentSearch->getResults();
	if (res.begin() == res.end()) {
		m_prevButton.setEnabled(false);
		m_nextButton.setEnabled(false);
		return;
	}
	if (m_iterator == std::list<address_t>::const_iterator() ||
			m_iterator == res.end()) {
		m_iterator = res.end();
		--m_iterator;
		jump();
	} else if ((--m_iterator) != res.end()) {
		jump();
	} else {
		wrap = true;
		m_iterator = res.end();
		--m_iterator;
		jump();
	}
	m_wrapLabel.setVisible(wrap);
}

void QTSearchWidget::nextResult()
{
	if (!m_currentSearch) {
		startSearch();
		return;
	}
	bool wrap = false;

	m_searching = false;
	const std::list<address_t> &res = m_currentSearch->getResults();
	if (res.begin() == res.end()) {
		m_prevButton.setEnabled(false);
		m_nextButton.setEnabled(false);
		return;
	}
	if (m_iterator == std::list<address_t>::const_iterator() ||
			m_iterator == res.end()) {
		m_iterator = res.begin();
		jump();
	} else if (++m_iterator == res.end()) {
		wrap = true;
		m_iterator = res.begin();
		jump();
	} else {
		jump();
	}
	m_wrapLabel.setVisible(wrap);
}

void QTSearchWidget::invalidate(const QString &s)
{
	if (s.isEmpty()) {
		m_prevButton.setEnabled(false);
		m_nextButton.setEnabled(false);
	} else {
		m_prevButton.setEnabled(true);
		m_nextButton.setEnabled(true);
	}
	stopSearch();
}

void QTSearchWidget::timeout()
{
	bool updated = false;
	if (!m_currentSearch || m_currentSearch->jobFinished()) {
		m_timer->stop();
		updated = true;
	} else if (m_currentSearch->count() != m_lastcount) {
		m_lastcount = m_currentSearch->count();
		updated = true;
	}
	if (updated &&  m_searching) {
		m_prevButton.setEnabled(true);
		m_nextButton.setEnabled(true);
		nextResult();
	}

	if (m_searchQueue.size() != 0) {
		while (m_searchQueue.size() != 0 && m_searchQueue.front()->jobFinished()) {
			SearchResults *sr = m_searchQueue.front();
			m_searchQueue.pop();
			delete sr;
		}
	}
}

/////////////////////////

QTFlatButton::QTFlatButton(const QIcon &i, const QString &name, QWidget *parent)
 : QPushButton(i, name, parent)
{
	setFlat(true);
}

void QTFlatButton::enterEvent(QEvent *e)
{
	setFlat(false);
	QPushButton::enterEvent(e);
}

void QTFlatButton::leaveEvent(QEvent *e)
{
	setFlat(true);
	QPushButton::leaveEvent(e);
}
