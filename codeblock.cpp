/*
 *  codeblock.cpp
 *  dismember
 *
 *  Created by David Carne on 12/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "codeblock.h"
#include "memlocdata.h"
#include "instruction.h"
#include "xref.h"

void linkCodeBlocks(CodeBlock * pre, CodeBlock * post)
{
	post->m_pre.insert(pre);
	pre->m_post.insert(post);
}

void unlinkCodeBlocks(CodeBlock * pre, CodeBlock * post)
{
	post->m_pre.erase(pre);
	pre->m_post.erase(post);
}

bool hasCodeXref(const Trace * t, xref_map_ci begin, xref_map_ci end)
{

	
	for (xref_map_ci i=begin; i != end; i++)
	{
		Xref * xr = (*i).second;
		uint32_t type = xr->get_type();
		if ((type == Xref::XR_TYPE_JMP) || (type == Xref::XR_TYPE_FNCALL))
			return true;
	}
	return false;
}

static void buildCodeBlockAt(const Trace * t, address_t growFrom)
{
	address_t current_start = growFrom;
	address_t current_end = growFrom;
	
	if (!dynamic_cast<Instruction *>(t->lookup_memloc(growFrom)))
		return;
		
	// while we're not at the beginning of the memory segment,
	// and while we don't have any code xrefs to this location,
	// we need to continue to loop
	// We may break out of the loop if the insn before has an xref from that is code 
	while (!addressIsBeginningOfMemory(t, current_start) &&
			!hasCodeXref(t, t->xref_to_lower_bound(current_start), t->xref_to_upper_bound(current_start))
		   )
	{
		
	}
}

void codeBlockAnalysisPass(const Trace * t, address_t growFrom)
{
		   
}