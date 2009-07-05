#include <QColor>
#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include "document.h"
#include "documentproxymodel.h"
#include "../memlocdata.h"
#include "program_flow_analysis.h"

#define log(a, x, y) do { printf("fn call: %s v:%d,%d\n", #a, (x), (y)); } while (0)

DocumentProxyModel::DocumentProxyModel(Document &doc)
 : QAbstractItemModel(NULL), m_doc(doc)
{
	m_gprox = new GuiProxy(m_doc.getTrace());
}

DocumentProxyModel::~DocumentProxyModel()
{ }

int DocumentProxyModel::rowCount(const QModelIndex &parent) const
{
	return m_gprox->getLineCount();
}

int DocumentProxyModel::columnCount(const QModelIndex & parent) const
{
	return 5;
}

QModelIndex DocumentProxyModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

QModelIndex DocumentProxyModel::index(int row, int column,
		const QModelIndex &parent) const
{
	log(DocumentProxyModel::index, row, column);
	return createIndex(row, column);
}

QString DocumentProxyModel::displayText(address_t addr) const
{
	MemlocData *id = m_doc.getTrace()->lookup_memloc(addr);
	u8 ch;
	if (id) {
		return QString(id->get_textual().c_str());
	} else if (m_doc.getTrace()->readByte(addr, &ch)) {
		char text[32], *p;
		p = text + snprintf(text, 32, "0x%02x", ch);
		if (isprint(ch))
			snprintf(p, 32 - (p - text), " '%c'", ch);
		return QString(".db\t").append(text);
	}
	return QString();
}

QString DocumentProxyModel::displayComment(address_t addr) const
{
	const Comment *cmt = m_doc.getTrace()->lookup_comment(addr);
	if (cmt)
		return QString(cmt->get_comment().c_str());
	return QString();
}

QString DocumentProxyModel::displayXrefs(address_t addr) const
{
	MemlocData *id = m_doc.getTrace()->lookup_memloc(addr);
	if (id && id->has_xrefs_to()) {
		QString str("");
		for (XrefManager::xref_map_ci j = id->begin_xref_to();
				j != id->end_xref_to(); j++) {
			Xref * x = (*j).second;
			char buf[256];
			snprintf(buf, 256, "; xref %08lx\t %x\n",
					x->get_src_addr(), x->get_type());
			str.append(buf);
		}
	}
	return QString();
}

QString DocumentProxyModel::displaySymbol(address_t addr) const
{
	const Symbol *sym = m_doc.getTrace()->lookup_symbol(addr);
	if (sym)
		return QString(sym->get_name().c_str()).append(":");
	return QString();
}

QVariant DocumentProxyModel::data(const QModelIndex &index, int role) const
{
	char data[256];
	switch (role) {
	case Qt::FontRole:
		break;
	case Qt::SizeHintRole: {
		QFontMetrics fm = QApplication::fontMetrics();
		return QVariant(QSize(0, fm.height()));
		} break;
	case Qt::BackgroundRole:
		if (index.row() & 1)
			return QVariant(QColor(245,245,245));
		else
			return QVariant(QColor(255,255,255));
	case Qt::ForegroundRole:
		try {
			address_t addr = m_gprox->getLineAddr(index.row());
			switch (index.column()) {
			case 0:
				return QVariant(QColor(0, 0, 0));
			case 1:
				return QVariant(QColor(0, 0, 255));
			case 2:
				if (m_doc.getTrace()->lookup_memloc(addr))
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
			address_t addr = m_gprox->getLineAddr(index.row());
			switch (index.column()) {
			case 0:
				snprintf(data, 256, "%08lx:", addr);
				return QVariant(data);
			case 1:
				return QVariant(displaySymbol(addr));
			case 2:
				return QVariant(displayText(addr));
			case 3:
				return QVariant(displayXrefs(addr));
			case 4:
				return QVariant(displayComment(addr));
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

void DocumentProxyModel::flush()
{
	m_gprox->update();
	reset();
}

bool DocumentProxyModel::isDefined(int row)
{
	return m_doc.getTrace()->lookup_memloc(m_gprox->getLineAddr(row)) != 0;
}

void DocumentProxyModel::analyze(int row)
{
	ProgramFlowAnalysis::submitAnalysisJob(&m_doc,
			m_doc.getTrace()->getCodeDataType(),
			m_gprox->getLineAddr(row));
	//flush();
}

void DocumentProxyModel::undefine(int row)
{
	// todo: apparently Trace::undefine(addr) doesn't work
	//flush();
}


void DocumentProxyModel::setSymbol(int row, QString str)
{
	address_t addr = m_gprox->getLineAddr(row);
	m_doc.getTrace()->create_sym(str.toStdString(), addr);
	flush();
}

QString DocumentProxyModel::getSymbol(int row)
{
	address_t addr = m_gprox->getLineAddr(row);
	const Symbol *sym = m_doc.getTrace()->lookup_symbol(addr);
	return sym ? QString(sym->get_name().c_str()) : QString();
}
