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

#include <QPainter>
#include <QToolTip>

#include "workspace.h"
#include "i_memlocdata.h"
#include "memsegment.h"
#include "dataview.h"

#define dataColor QColor(0xb9,0x47,0x1b)
#define codeColor QColor(0x2d,0x36,0xbc)
#define unkColor  QColor(0xbc,0xb3,0x2d)
#define undColor  QColor(0x00,0x00,0x00)
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

	I_ProjectModel &t = m_model->getProjectModel();

	memsegment_ci mi;
	memsegment_ci begin = t.memsegs_begin();
	memsegment_ci end = t.memsegs_end();
	if (begin == end)
		return;
	
	bool tall = s.height() > s.width();
	int npixels = tall ? s.height() : s.width();
	m_paddrMap.clear();
	m_paddrMap.resize(npixels);

	uint64_t memSize = 0;
	for (mi = begin; mi != end; ++mi)
		memSize += (*mi)->get_length();

	float bytesPerPixel = (float)memSize / npixels;

	int pixel = 0;
	for (mi = begin; mi != end; ++mi) {
		address_t baseAddr = (*mi)->getBaseAddress();
		double off = 0.0;
		for (; pixel < npixels; ++pixel) {
			I_MemlocData *id;
			address_t normAddr = baseAddr + ROUND(off);

			if (!normAddr.isValid())
				break;

			if (!(*mi)->is_defined())
				painter.setPen(undColor);
			else if ((id = t.lookup_memloc(normAddr, false))) {
				if (id->is_executable())
					painter.setPen(codeColor);
				else
					painter.setPen(dataColor);
			} else
				painter.setPen(unkColor);
			
			if (tall)
				painter.drawLine(0, pixel, s.width(), pixel);
			else
				painter.drawLine(pixel, 0, pixel, s.height());

			m_paddrMap[pixel] = normAddr;
			off += bytesPerPixel;
		}
	}
}

void QTDataView::mousePressEvent(QMouseEvent *event)
{
	m_mouseActive = true;
	if (m_mouseInside) {
		QSize s = size();
		bool tall = s.height() > s.width();
		int pixel = tall ? event->pos().y() : event->pos().x();
		if (m_paddrMap.size() > (u32)pixel) {
			address_t addr = m_paddrMap[pixel];
			if (!addr.isValid())
				return;
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
		QSize s = size();
		bool tall = s.height() > s.width();
		int pixel = tall ? event->pos().y() : event->pos().x();
		if (m_paddrMap.size() > (u32)pixel) {
			address_t addr = m_paddrMap[pixel];
			if (!addr.isValid()) // whoops.
				return;
			QToolTip::showText(event->globalPos(),
				QString(addr.toString().c_str()), this, rect());
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
