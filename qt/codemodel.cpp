#include <QColor>
#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include "document.h"
#include "xref.h"
#include "memlocdata.h"

#include "codemodel.h"


QTCodeModel::QTCodeModel(QTRuntimeModel *model)
 : QAbstractItemModel(NULL), m_model(model)
{ }

QTCodeModel::~QTCodeModel()
{ }

int QTCodeModel::rowCount(const QModelIndex &parent) const
{
	return m_model->getProxy().getLineCount();
}

int QTCodeModel::columnCount(const QModelIndex & parent) const
{
	return 5;
}

QModelIndex QTCodeModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

QModelIndex QTCodeModel::index(int row, int column,
		const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QString QTCodeModel::displayText(Trace &t, address_t addr) const
{
	MemlocData *id = t.lookup_memloc(addr);
	u8 ch;
	if (id) {
		return QString(id->get_textual().c_str());
	} else if (t.readByte(addr, &ch)) {
		char text[32], *p;
		p = text + snprintf(text, 32, "0x%02x", ch);
		if (isprint(ch))
			snprintf(p, 32 - (p - text), " '%c'", ch);
		return QString(".db\t").append(text);
	}
	return QString();
}

QString QTCodeModel::displayComment(Trace &t, address_t addr) const
{
	const Comment *cmt = t.lookup_comment(addr);
	if (cmt)
		return QString(cmt->get_comment().c_str());
	return QString();
}

QString QTCodeModel::displayXrefs(Trace &t, address_t addr) const
{
	MemlocData *id = t.lookup_memloc(addr);
	if (id && id->has_xrefs_to()) {
		QString str("");
		for (XrefManager::xref_map_ci j = id->begin_xref_to();
				j != id->end_xref_to(); j++) {
			Xref * x = (*j).second;
			const char *type = "?";
			char buf[256];
			switch (x->get_type()) {
			case Xref::XR_TYPE_JMP:
				type = "jump";
				break;
			case Xref::XR_TYPE_FNCALL:
				type = "call";
				break;
			case Xref::XR_TYPE_DATA:
				type = "data";
				break;
			}
			snprintf(buf, 256, "xref: 0x%08x(%s)\n",
					(u32)x->get_src_addr(), type);
			str.append(buf);
		}
		str.chop(1);
		return str;
	}
	return QString();
}

QString QTCodeModel::displayXrefBrief(Trace &t, address_t addr) const
{
	MemlocData *id = t.lookup_memloc(addr);
	if (id && id->has_xrefs_to()) {
		char buf[256];
		QString str("; xrefs");
		snprintf(buf, 256, "(%d): ", id->count_xrefs_to());
		str.append(buf);
		for (XrefManager::xref_map_ci j = id->begin_xref_to();
				j != id->end_xref_to(); j++) {
			Xref * x = (*j).second;
			snprintf(buf, 256, "0x%08x,", (u32)x->get_src_addr());
			str.append(buf);
		}
		str.chop(1);
		return str;
	}
	return QString();
}

QString QTCodeModel::displaySymbol(Trace &t, address_t addr) const
{
	const Symbol *sym = t.lookup_symbol(addr);
	if (sym)
		return QString(sym->get_name().c_str()).append(":");
	return QString();
}

QVariant QTCodeModel::data(const QModelIndex &index, int role) const
{
	GuiProxy &gprox = m_model->getProxy();
	char data[256];
	switch (role) {
	case Qt::ToolTipRole:
		if (index.column() == 3) {
			address_t addr = gprox.getLineAddr(index.row());
			Trace &t = m_model->getTrace();
			return QVariant(displayXrefs(t, addr));
		}
		return QVariant();
		break;
	case Qt::FontRole:
		break;
	case Qt::BackgroundRole:
		if (index.row() & 1)
			return QVariant(QColor(245,245,245));
		else
			return QVariant(QColor(255,255,255));
	case Qt::ForegroundRole:
		try {
			address_t addr = gprox.getLineAddr(index.row());
			switch (index.column()) {
			case 0:
				return QVariant(QColor(0, 0, 0));
			case 1:
				return QVariant(QColor(0, 0, 255));
			case 2:
				if (m_model->getTrace().lookup_memloc(addr))
					return QVariant(QColor(0, 0, 200));
				else
					return QVariant(QColor(125, 125, 125));
			case 3:
			case 4:
				return QVariant(QColor(150, 150, 150));
			}
		} catch (std::out_of_range &e) {
			return QVariant();
		}
		break;
	case Qt::DisplayRole:
		try {
			address_t addr = gprox.getLineAddr(index.row());
			Trace &t = m_model->getTrace();
			switch (index.column()) {
			case 0:
				snprintf(data, 256, "%08x:", (u32)addr);
				return QVariant(QString(data));
			case 1:
				return QVariant(displaySymbol(t, addr));
			case 2:
				return QVariant(displayText(t, addr));
			case 3:
				return QVariant(displayXrefBrief(t, addr));
			case 4:
				return QVariant(displayComment(t, addr));
			}
		} catch (std::out_of_range &e) {
			return QVariant();
		}
		break;
	default:
		return QVariant();
	}

	return QVariant();
}

void QTCodeModel::flush()
{
	reset();
}
