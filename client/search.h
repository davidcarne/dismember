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

#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "run_queue.h"
#include "workspace.h"
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
	static sp_RunQueueJob createSearchJob(Workspace * d,
			uint8_t *bytes, psize_t len, SearchResults **res);
	static SearchResults *submitSearchJob(Workspace * d,
			uint8_t *bytes, psize_t len);

 private:
	static bool search(Workspace * d, uint8_t *bytes,
			psize_t len, SearchResults *sr);
};

#endif
