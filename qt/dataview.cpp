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

QTDataView::QTDataView(QWidget *parent)
 : QWidget(parent), m_model(NULL), m_mouseActive(false), m_mouseInside(false)
{ }

void QTDataView::setRuntimeModel(QTRuntimeModel *rt)
{
	m_model = rt;
}

void QTDataView::runtimeUpdated(QTRuntimeEvent *m)
{
	update();
}

void QTDataView::paintEvent(QPaintEvent *event)
{
	if (!m_model) return;

	QPainter painter(this);
	QSize s = size();

	Trace &t = m_model->getTrace();

	MemSegmentManager::memseglist_ci mi;
	MemSegmentManager::memseglist_ci begin = t.memsegs_begin();
	MemSegmentManager::memseglist_ci end = t.memsegs_end();
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
		MemlocData *id = t.lookup_memloc(normAddr, false);
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

void QTDataView::mousePressEvent(QMouseEvent *event)
{
	m_mouseActive = true;
	if (m_mouseInside) {
		if (m_paddrMap.size() > (u32)event->pos().y()) {
			address_t addr = m_paddrMap[event->pos().y()];
			m_model->postJump(addr);
		}
	}
}

void QTDataView::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseActive = false;
}

void QTDataView::mouseMoveEvent(QMouseEvent *event)
{
	if (m_mouseActive && m_mouseInside) {
		if (m_paddrMap.size() > (u32)event->pos().y()) {
			address_t addr = m_paddrMap[event->pos().y()];
			char buf[256];
			snprintf(buf, 256, "0x%08x", (u32)addr);
			QToolTip::showText(event->globalPos(),
				QString(buf), this, rect());
			m_model->postJump(addr);
		}
	}
}

void QTDataView::enterEvent(QEvent *event)
{
	m_mouseInside = true;
}

void QTDataView::leaveEvent(QEvent *event)
{
	m_mouseInside = false;
}
