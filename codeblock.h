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

#ifndef _CODEBLOCK_H_
#define _CODEBLOCK_H_


#include <set>
#include "types.h"
#include "i_projectmodel.h"

class CodeBlock;

class CodeBlock {
private:
	address_t m_start;
	address_t m_end; // points to the addr after the block of code
	typedef std::set< CodeBlock * > codeblockset_t;
	codeblockset_t m_pre;
	codeblockset_t m_post;
	I_ProjectModel * m_trace;
	
	
	CodeBlock(I_ProjectModel * t, address_t start, address_t end);
	friend class CodeBlockManager;
	
public:
	address_t getStart() {
		return m_start;
	};
	
	address_t getEnd() {
		return m_end;
	};
};

class CodeBlockManager {
private:
	void linkCodeBlocks(CodeBlock * pre, CodeBlock * post);
	void unlinkCodeBlocks(CodeBlock * pre, CodeBlock * post);
	typedef std::map<address_t, CodeBlock *, address_t::less> blocklist_t;
	typedef blocklist_t::iterator blockiterator;
	blocklist_t m_blocks;
	
	CodeBlock * buildCodeBlockAt(MemlocData * growFrom);
	I_ProjectModel * m_trace;
	
	// Hooks for change notifications
public:
	void codeBlockAnalysisPassAt(address_t growFrom = address_t());
	CodeBlock * codeBlockAt(address_t codeBlock);
};


#endif

