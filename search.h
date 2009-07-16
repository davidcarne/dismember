
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "run_queue.h"
#include "document.h"
#include "types.h"
#include <list>

class SearchResults
{
 public:
	~SearchResults();

	/**
	 * Convienience function to return the current number of results.
	 */
	int count();

	/**
	 * Returns whether the associated search job is finished.
	 */
	bool jobFinished();

	/**
	 * Returns the result list for search.
	 */
	const std::list<address_t> &getResults();

 private:
	friend class Search;

	SearchResults();
	void addResult(address_t addr);
	void removeResult(address_t addr);
	void setJobFinished(bool fin);

	bool m_jobFinished;
	std::list<address_t> m_results;
};

class Search
{
 public:
	static sp_RunQueueJob createSearchJob(Document * d,
			uint8_t *bytes, psize_t len, SearchResults **res);
	static SearchResults *submitSearchJob(Document * d,
			uint8_t *bytes, psize_t len);

 private:
	static bool search(Document * d, uint8_t *bytes,
			psize_t len, SearchResults *sr);
};

#endif
