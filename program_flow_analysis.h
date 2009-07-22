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


