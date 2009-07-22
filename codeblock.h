/*
 *  codeblock.h
 *  dismember
 *
 *  Created by David Carne on 12/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CODEBLOCK_H_
#define _CODEBLOCK_H_


#include <set>
#include "types.h"
#include "dsmem_trace.h"

class CodeBlock;

class CodeBlock {
private:
	address_t m_start;
	address_t m_end; // points to the addr after the block of code
	typedef std::set< CodeBlock * > codeblockset_t;
	codeblockset_t m_pre;
	codeblockset_t m_post;
	ProjectModel * m_trace;
	
	
	CodeBlock(ProjectModel * t, address_t start, address_t end);
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
	ProjectModel * m_trace;
	
	// Hooks for change notifications
public:
	void codeBlockAnalysisPassAt(address_t growFrom = address_t());
	CodeBlock * codeBlockAt(address_t codeBlock);
};


#endif

