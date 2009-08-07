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

#include "i_projectmodel.h"
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

sp_RunQueueJob Search::createSearchJob(Workspace *d, uint8_t *b,
		psize_t len, SearchResults **res)
{
	SearchResults *sr = new SearchResults();
	if (res)
		*res = sr;
	FunctorRunQueueJob::jobfun_t jb =
			boost::bind(&Search::search, d, b, len, sr);
	return createFunctorRunQueueJob("search", -1, jb);
}
SearchResults *Search::submitSearchJob(Workspace *d, uint8_t *b, psize_t len)
{
	SearchResults *res;
	d->getRunQueue()->submit(createSearchJob(d, b, len, &res));
	return res;
}

bool Search::search(Workspace * d, uint8_t *b, psize_t len, SearchResults *sr)
{
	I_ProjectModel *t = d->getProjectModel();

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
