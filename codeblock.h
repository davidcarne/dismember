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
#include "trace.h"

class CodeBlock;

class CodeBlock {
private:
	address_t m_start;
	address_t m_end; // points to the addr after the block of code
	typedef std::set< CodeBlock * > codeblockset_t;
	codeblockset_t m_pre;
	codeblockset_t m_post;
	Trace * t;
	
public:
	CodeBlock(Trace * t, address_t m_start, address_t m_end);
	friend void linkCodeBlocks(CodeBlock * pre, CodeBlock * post);
	friend void unlinkCodeBlocks(CodeBlock * pre, CodeBlock * post);
	
	address_t getStart() {
		return m_start;
	};
	
	address_t getEnd() {
		return m_end;
	};
};

void linkCodeBlocks(CodeBlock * pre, CodeBlock * post);
void unlinkCodeBlocks(CodeBlock * pre, CodeBlock * post);

void codeBlockAnalysisPass(Trace * t, address_t growFrom = -1);
#endif