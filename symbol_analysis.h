#ifndef _SYMBOL_ANALYSIS_H_
#define _SYMBOL_ANALYSIS_H_

#include "run_queue.h"
#include "document.h"

class SymbolAnalysis
{
 public:
	static sp_RunQueueJob createAnalysisJob(Document * d);
	static void submitAnalysisJob(Document * d);

 private:
	static bool analyze(Document * d);
};

#endif
