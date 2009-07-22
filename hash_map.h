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

#ifndef _HASH_MAP_FIX_H_
#define _HASH_MAP_FIX_H_

#ifdef __GNUC__
# if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) || (__GNUC__ > 4)
#  include <tr1/unordered_map>
#  include "types.h"
namespace std
{
	template<class Key, class T>
	class hash_map
	 : public std::tr1::unordered_map<Key, T>
	{ };

	namespace tr1 {
		template<> struct hash< address_t >
		{
			size_t operator()(const address_t &x) const
			{
				uint64_t __x = x.getValue();
				return (__x >> 32) ^ __x;
			}
		};
	}
}
# else
#  include <ext/hash_map>
#  include <string>
#  include "types.h"
namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()(const std::string &x) const
		{
			return __stl_hash_string(x.c_str());
		}
	};

	template<> struct hash< address_t >
	{
		size_t operator()(const address_t &x) const
		{
			uint64_t __x = x.getValue();
			return (__x >> 32) ^ __x;
		}
	};

}
namespace std
{
	using namespace __gnu_cxx;
}
# endif
#else
#include <hash_map>
#endif


#endif
