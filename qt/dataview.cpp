#include <QPainter>
#include <QToolTip>

#include "document.h"
#include "memlocdata.h"
#include "memsegment.h"
#include "dataview.h"

#define dataColor QColor(0xb9,0x47,0x1b)
#define codeColor QColor(0x2d,0x36,0xbc)
#define unkColor  QColor(0xbc,0xb3,0x2d)
#define segColor  QColor(0x22,0x66,0x22)
#define ROUND(x) ((int)((x)+0.5f))

DataView::DataView(QWidget *parent)
 : QWidget(parent), m_doc(NULL), m_mouseActive(false), m_mouseInside(false)
{ }

void DataView::setDocument(Document *doc)
{
	m_doc = doc;
	flush();
}

void DataView::flush()
{
	update();
}

void DataView::paintEvent(QPaintEvent *event)
{
	if (!m_doc) return;

	QPainter painter(this);
	QSize s = size();

	Trace *t = m_doc->getTrace();

	MemSegmentManager::memseglist_ci mi;
	MemSegmentManager::memseglist_ci begin = t->memsegs_begin();
	MemSegmentManager::memseglist_ci end = t->memsegs_end();
	if (begin == end)
		return;
	
	m_paddrMap.clear();
	m_paddrMap.resize(s.height());

	address_t firstAddr = (*begin)->get_start();
	address_t memSize = 0;
	for (mi = begin; mi != end; ++mi)
		memSize += (*mi)->get_length();

	float bytesPerPixel = (float)memSize / s.height();

	mi = begin;
	double addr = firstAddr;
	for (int i = 0; i < s.height(); ++i) {
		address_t normAddr = ROUND(addr);
		MemlocData *id = t->lookup_memloc(normAddr, false);
		if (id) {
			if (id->is_executable())
				painter.setPen(codeColor);
			else
				painter.setPen(dataColor);
		} else
			painter.setPen(unkColor);
		painter.drawLine(0, i, s.width(), i);

		m_paddrMap[i] = normAddr;
		addr += bytesPerPixel;

		MemSegment *mSeg = *mi;
		if (addr > mSeg->get_start() + mSeg->get_length()) {
			painter.setPen(segColor);
			painter.drawLine(0, i, s.width()/3, i);
			++mi;
		}
	}
}

void DataView::mousePressEvent(QMouseEvent *event)
{
	m_mouseActive = true;
	if (m_mouseInside) {
		if (m_paddrMap.size() > (u32)event->pos().y()) {
			address_t addr = m_paddrMap[event->pos().y()];
		}
	}
}

void DataView::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseActive = false;
}

void DataView::mouseMoveEvent(QMouseEvent *event)
{
	if (m_mouseActive && m_mouseInside) {
		if (m_paddrMap.size() > (u32)event->pos().y()) {
			address_t addr = m_paddrMap[event->pos().y()];
			char buf[256];
			snprintf(buf, 256, "0x%08x", (u32)addr);
			QToolTip::showText(event->globalPos(),
				QString(buf), this, rect());
		}
	}
}

void DataView::enterEvent(QEvent *event)
{
	m_mouseInside = true;
}

void DataView::leaveEvent(QEvent *event)
{
	m_mouseInside = false;
}
