/*
 *  bind_analysis.cpp
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <boost/python.hpp>
using namespace boost::python;

#include "program_flow_analysis.h"

void bind_analysis()
{
	def("progflow_createjob", &ProgramFlowAnalysis::createAnalysisJob);
	def("progflow_submitjob", &ProgramFlowAnalysis::submitAnalysisJob);
	//def("progflow_analyze", &ProgramFlowAnalysis::analyze);
	//def("progflow_undefine", &ProgramFlowAnalysis::undefine);
}