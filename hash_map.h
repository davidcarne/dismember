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
