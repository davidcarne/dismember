#include "dsmem_trace.h"
#include "search.h"
#include "memsegment.h"
#include "memsegmentmanager.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

SearchResults::~SearchResults()
{ }

/**
 * Convienience function to return the current number of results.
 */
int SearchResults::count()
{
	return m_results.size();
}

/**
 * Returns whether the associated search job is finished.
 */
bool SearchResults::jobFinished()
{
	return m_jobFinished;
}

/**
 * Returns the result list for search.
 */
const std::list<address_t> &SearchResults::getResults()
{
	return m_results;
}

SearchResults::SearchResults()
 : m_jobFinished(false)
{ }

void SearchResults::addResult(address_t addr)
{
	m_results.push_back(addr);
}

void SearchResults::removeResult(address_t addr)
{
	m_results.remove(addr);
}

void SearchResults::setJobFinished(bool fin)
{
	m_jobFinished = fin;
}

sp_RunQueueJob Search::createSearchJob(Document *d, uint8_t *b,
		psize_t len, SearchResults **res)
{
	SearchResults *sr = new SearchResults();
	if (res)
		*res = sr;
	FunctorRunQueueJob::jobfun_t jb =
			boost::bind(&Search::search, d, b, len, sr);
	return createFunctorRunQueueJob("search", -1, jb);
}
SearchResults *Search::submitSearchJob(Document *d, uint8_t *b, psize_t len)
{
	SearchResults *res;
	d->getRunQueue()->submit(createSearchJob(d, b, len, &res));
	return res;
}

bool Search::search(Document * d, uint8_t *b, psize_t len, SearchResults *sr)
{
	Trace *t = d->getTrace();

	MemSegmentManager::memseglist_ci mi;
	MemSegmentManager::memseglist_ci begin = t->memsegs_begin();
	MemSegmentManager::memseglist_ci end = t->memsegs_end();
	if (begin == end)
		return true;
	
	mi = begin;
	address_t addr = (*mi)->getBaseAddress();
	while (mi != end) {
		uint8_t byte;
		psize_t pos = 0;
		address_t saddr = addr;
		for (; t->readByte(saddr, &byte) && byte == b[pos]; ++saddr) {
			if (++pos == len) {
				sr->addResult(addr);
				addr = saddr + 1;
				break;
			}
		}
		if (!(++addr).isValid()) {
			if (++mi != end)
				addr = (*mi)->getBaseAddress();
		}
	}
	sr->setJobFinished(true);
	return true;
}
