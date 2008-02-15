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

static void buildCodeBlockAt(const Trace * t, MemlocData * growFrom)
{
	MemlocData * current_start = growFrom;
	MemlocData * current_start_prev = growFrom;
	MemlocData * current_end = growFrom;
	
	if (!dynamic_cast<Instruction *>(growFrom))
		return;
		
	// while we're not at the beginning of the memory segment,
	// and while we don't have any code xrefs to this location,
	// we need to continue to loop
	// We may break out of the loop if the insn before has an xref from that is code 
	while ((current_start_prev = current_start->getPreviousContiguous()) &&
		   !hasCodeXref(t, current_start->begin_xref_to(), current_start->end_xref_to()) &&
		   !hasCodeXref(t, current_start_prev->begin_xref_from(), current_start->begin_xref_from())
		   )
		current_start = current_start_prev;
	current_end = current_start;
}

void codeBlockAnalysisPass(Trace * t, address_t growFrom)
{
	if (growFrom == (address_t)-1)
	{
		return;
	}
	buildCodeBlockAt(t, t->lookup_memloc(growFrom,false));
}