/*
 *  memsegmentmanager.h
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _memsegmentmanager_H_
#define _memsegmentmanager_H_
#include <set>
#include "MemSegment.h"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

class MemSegmentManager {
public:
	MemSegmentManager();
	
	typedef std::set<MemSegment *, MemSegment::less> memseglist_t;
	typedef memseglist_t::const_iterator memseglist_ci;
	
	bool addSegment(MemSegment * m);
	bool readByte(address_t taddr, uint8_t * data) const;
	bool readBytes(address_t, u8, u8*) const;
	memseglist_ci memsegs_begin() const;
	memseglist_ci memsegs_end() const;
private:
	
	template<class Archive> void serialize(Archive & ar, const unsigned int)
	{
		ar & m_mem_segments;
	}
	
	friend class boost::serialization::access;
	
	/* Needs serializing */
	memseglist_t m_mem_segments;
	
	/* do not serialize - cache*/
	mutable MemSegment * m_last_segment;
};

#endif


