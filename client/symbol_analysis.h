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

#ifndef _SYMBOL_ANALYSIS_H_
#define _SYMBOL_ANALYSIS_H_

#include "run_queue.h"
#include "workspace.h"

class SymbolAnalysis
{
 public:
	static sp_RunQueueJob createAnalysisJob(Workspace * d);
	static void submitAnalysisJob(Workspace * d);

 private:
	static bool analyze(Workspace * d);
};

#endif
