#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <errno.h>
#include <assert.h>
#include <stack>

#include "dsmem_trace.h"
#include "memlocdata.h"
#include "binaryconstant.h"
#include "stringconstant.h"
#include "datatype.h"
#include "xref.h"
#include "instruction.h"

//#include "memtags.h"
#include "architecture.h"
#include "memsegment.h"

#define ASSERT_RESOLVE(addr) assert(readByte(addr, NULL))

ProjectModel::ProjectModel(Architecture * arch) : m_arch(arch), m_xrefmanager(this)
{}

DataType * ProjectModel::getCodeDataType() {
	return m_arch->getDataType(this);
}


bool ProjectModel::addSegment(MemSegment * m)
{
	/** \todo notifications need to be reworked, a global notification is a bit of a hack. Also, a dedicated memory segment cat would be good */
	notifyMemlocChange(NULL, HOOK_MODIFY);
	return m_memsegmentmanager.addSegment(m);
}


bool ProjectModel::readByte(address_t taddr, uint8_t * data) const
{
	return m_memsegmentmanager.readByte(taddr, data);
}

bool ProjectModel::readBytes(address_t taddr, u8 len, u8* data) const
{
	
	return m_memsegmentmanager.readBytes(taddr, len, data);
}

MemSegmentManager::memseglist_ci ProjectModel::memsegs_begin() const
{
	return m_memsegmentmanager.memsegs_begin();
}

MemSegmentManager::memseglist_ci ProjectModel::memsegs_end() const
{
	return m_memsegmentmanager.memsegs_end();
}

address_t ProjectModel::locateAddress(uint64_t address) const
{
	return m_memsegmentmanager.locateAddress(address);
}

MemlocData * ProjectModel::createMemlocDataAt(DataType * d, address_t addr)
{
	assert(d);
	MemlocData * i = d->instantiate(addr);
	if (!i)
		return NULL;
	
	m_memlocmanager.insertMemloc(i);
	
	// Todo: this is sorta hacky....  the memlocd's should insert their own xrefs, or provide a way to enumerate them
	Instruction * b = dynamic_cast<Instruction*>(i);
	
	// Create Code XREF's if this memlocdata has any
	// The fn / jmp business is to avoid a gcc bug
	Xref::xref_type_e fn = Xref::XR_TYPE_FNCALL;
	Xref::xref_type_e jmp = Xref::XR_TYPE_JMP;
	if (b && (b->get_pcflags() & Instruction::PCFLAG_LOCMASK) == Instruction::PCFLAG_DIRLOC)
		create_xref(b->get_addr(), b->get_direct_jump_addr(), b->get_pcflags() & Instruction::PCFLAG_DIRLK ? fn : jmp);
	
	if (b && (b->get_pcflags() & Instruction::PCFLAG_DREF))
		create_xref(b->get_addr(), b->get_data_ref_addr(), Xref::XR_TYPE_DATA);
	
	notifyMemlocChange(i, HOOK_CREATE);
	
	return i;
}

ProjectModel::~ProjectModel()
{
}

/* Symbol accessors */
SymbolList::symname_ci ProjectModel::sym_begin_name() const
{
	return m_symlist.begin_name();
}

SymbolList::symname_ci ProjectModel::sym_end_name() const
{
	return m_symlist.end_name();
}

SymbolList::symaddr_ci ProjectModel::sym_begin_addr() const
{
	return m_symlist.begin_addr();
}

SymbolList::symaddr_ci ProjectModel::sym_end_addr() const
{
	return m_symlist.end_addr();
}

Symbol * ProjectModel::find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order) const
{
	return m_symlist.find_ordered_symbol(index, order);
}

uint32_t ProjectModel::get_symbol_count(void) const
{
	return  m_symlist.get_symbol_count();
}

const Symbol * ProjectModel::lookup_symbol(const std::string & symname) const
{
	const Symbol *sy = m_symlist.get_symbol(symname);
	return sy;
}

const Symbol * ProjectModel::lookup_symbol(address_t addr) const
{
	const Symbol *sy = m_symlist.get_symbol(addr);
	return sy;
}


MemlocData * ProjectModel::lookup_memloc(address_t addr, bool exactmatch) const
{	
	return m_memlocmanager.findMemloc(addr, exactmatch);
}

void ProjectModel::remove_memloc(address_t addr)
{
	MemlocData * a = m_memlocmanager.findMemloc(addr, true);
	if (!a)
		return;
	
	notifyMemlocChange(a, HOOK_DELETE);
	
	m_memlocmanager.removeMemloc(a);
	
	
	delete a;
}

MemlocManager::memloclist_ci ProjectModel::memloc_list_begin() const
{
	return m_memlocmanager.memloc_list_begin();
}

MemlocManager::memloclist_ci ProjectModel::memloc_list_end() const
{
	return m_memlocmanager.memloc_list_end();
}

Comment *ProjectModel::create_comment(std::string cmt, address_t addr)
{
	return m_commentlist.set_comment(addr, cmt);
}

Comment *ProjectModel::lookup_comment(address_t addr) const
{
	return m_commentlist.get_comment(addr);
}

Symbol *ProjectModel::create_sym(const std::string & name, address_t addr)
{

	//MemlocData * memd = lookup_memloc(addr);
	Symbol * sym = m_symlist.set_symbol(addr, name);
	if (sym != NULL)
		notifySymbolChange(sym, HOOK_CREATE);
	//if (memd)
	//	memd->set_symbol(sym);
	return sym;
}


DataTypeReg::datatypereg_ci ProjectModel::getDataTypeBegin() const
{
	return m_datatypelist.getBegin();
}

DataTypeReg::datatypereg_ci ProjectModel::getDataTypeEnd() const
{
	return m_datatypelist.getEnd();
}
	
sp_DataType ProjectModel::lookupDataType(std::string name) const
{
	return m_datatypelist.lookupDataType(name);
}

void ProjectModel::addDataType(sp_DataType d)
{
	assert(d);
	assert(d->getName().size() > 0);
	return m_datatypelist.insertDataType(d->getName(), d);
}


Xref *ProjectModel::create_xref(address_t srcaddr, address_t dstaddr, Xref::xref_type_e type)
{
	
	MemlocData * src = lookup_memloc(srcaddr);
	MemlocData * dst = lookup_memloc(dstaddr);
	
	if (!src)
		return NULL;
	
	Xref * sx = m_xrefmanager.createXref(srcaddr, dstaddr, type);
	
	
	// TODO: This should be attached as a notifier, maybe?
	// Build data if its a data xref
	if (type == Xref::XR_TYPE_DATA)
	{
		Instruction * id  = dynamic_cast<Instruction *>(src);
		if (id)
			switch (id->get_pcflags() & Instruction::PCFLAG_DSMASK) {
					// HACK HACK HACK - changeme
				case Instruction::PCFLAG_DSBYTE:
					createMemlocDataAt(lookupDataType("u8_le").get(), id->get_data_ref_addr());
					break;
				
				case Instruction::PCFLAG_DSWORD:
					createMemlocDataAt(lookupDataType("u32_le").get(), id->get_data_ref_addr());
					break;
			}
	}
	
	// Mark as a function call
	if (dst)
	{
		if (type == Xref::XR_TYPE_FNCALL)
		{
			Instruction * instdst;
			instdst = dynamic_cast<Instruction *>(dst);
			if (instdst)
				instdst->mark_fn_ent(true);
		}
		
	}
	

	
	return sx;
}

