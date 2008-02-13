/*
 *  string_hash.h
 *  dismember
 *
 *  Created by David Carne on 13/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _string_hash_H_
#define _string_hash_H_

#include <string>

/* Hack hack hack.. stupid stl */
namespace __gnu_cxx
{
	/**
	 * @cond NEVER
	 */
	
	/**
	 * \brief hack to make gcc string hashing work
	 */
	template<> struct hash< std::string >
	{
		/** hack to make gcc string hashing work */
		size_t operator()( const std::string& x ) const
		{
			return __stl_hash_string( x.c_str() );
		}
	};
	
	/**
	 * @endcond
	 */
}

#endif


