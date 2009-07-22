/*
 *  program_flow_analysis.h
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _program_flow_analysis_H_
#define _program_flow_analysis_H_
#include "dsmem_trace.h"
#include "datatype.h"
#include "run_queue.h"
#include "document.h"

class ProgramFlowAnalysis {
public:
	static sp_RunQueueJob createAnalysisJob(Workspace * d, DataType * dtcreate, address_t start);
	static void submitAnalysisJob(Workspace * d, DataType * dtcreate, address_t start);

	static sp_RunQueueJob createUndefineJob(Workspace * d, address_t start);
	static void submitUndefineJob(Workspace * d, address_t start);
	
private:
	static bool undefine(Workspace * d, address_t start);
	static bool analyze(Workspace * d, DataType * dtcreate, address_t start);
};

#endif


