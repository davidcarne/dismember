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

class ProgramFlowAnalysis {
	static bool ProgramFlowAnalysis::undefine(Trace * t, address_t start);
	static bool analyze(Trace * t,  address_t start);
};

#endif


