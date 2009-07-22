#ifndef _SYMBOL_ANALYSIS_H_
#define _SYMBOL_ANALYSIS_H_

#include "run_queue.h"
#include "document.h"

class SymbolAnalysis
{
 public:
	static sp_RunQueueJob createAnalysisJob(Workspace * d);
	static void submitAnalysisJob(Workspace * d);

 private:
	static bool analyze(Workspace * d);
};

#endif
