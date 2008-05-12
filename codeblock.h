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
	Trace * m_trace;
	
	
	CodeBlock(Trace * t, address_t start, address_t end);
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
	typedef std::map<address_t, CodeBlock *>::iterator blockiterator;
	std::map<address_t, CodeBlock *> m_blocks;
	
	CodeBlock * buildCodeBlockAt(MemlocData * growFrom);
	Trace * m_trace;
	
	// Hooks for change notifications
public:
	void codeBlockAnalysisPassAt(address_t growFrom = -1);
	CodeBlock * codeBlockAt(address_t codeBlock);
};


#endif

