/*  Copyright 2009 David Carne
 *
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

#ifndef _KVS_TEMPLATES_H_
#define _KVS_TEMPLATES_H_

#include <map>
#include <boost/thread/recursive_mutex.hpp>

template <typename Tp, typename Tsrc> class locking_iterator_t {
	public:
		locking_iterator_t(boost::recursive_mutex & mutex, Tsrc & src) : m_mutex(mutex), m_src(src), m_lock(m_mutex)
		{
		}
		
		locking_iterator_t(const locking_iterator_t & i) : m_mutex(i.m_mutex), m_src(i.m_src), m_lock(m_mutex)
		{}
		
	private:
		const locking_iterator_t & operator=(const locking_iterator_t & i)
		{
			
		}
	public:
		void operator++(int n)
		{
			m_src.operator++(n);
		}
		
		Tp operator*()
		{
			return *m_src;
		}
	
	
	bool operator==(locking_iterator_t<Tp,Tsrc> & i) const
	{
		return m_src == i.m_src;
	}
	
	bool operator!=(const locking_iterator_t<Tp,Tsrc> & i) const
	{
		return m_src != i.m_src;
	}
	
	public:
		Tsrc m_src;
	
	boost::recursive_mutex & m_mutex;
	boost::recursive_mutex::scoped_lock m_lock;
	
};


template <typename Tp, typename Tsrc> class const_locking_iterator_t {
public:
	const_locking_iterator_t(boost::recursive_mutex & mutex, Tsrc & src) : m_mutex(mutex), m_src(src), m_lock(m_mutex)
	{
	}
	
	const_locking_iterator_t(const const_locking_iterator_t<Tp, Tsrc> & i) : m_mutex(i.m_mutex), m_src(i.m_src), m_lock(m_mutex)
	{}
	
	const_locking_iterator_t(const locking_iterator_t<Tp, Tsrc> & i) : m_mutex(i.m_mutex), m_src(i.m_src), m_lock(m_mutex)
	{}
	
	
private:
	
	
	const_locking_iterator_t()
	{
	}
	const const_locking_iterator_t & operator=(const const_locking_iterator_t & i)
	{
		
	}
public:
	void operator++(int n)
	{
		m_src.operator++(n);
	}
	
	Tp operator*()
	{
		return *m_src;
	}
	
	
	bool operator==(const const_locking_iterator_t<Tp,Tsrc> & i) const
	{
		return m_src == i.m_src;
	}
	
	bool operator!=(const const_locking_iterator_t<Tp,Tsrc> & i) const
	{
		return m_src != i.m_src;
	}
	
public:
	Tsrc m_src;
	
	boost::recursive_mutex & m_mutex;
	boost::recursive_mutex::scoped_lock m_lock;
	
	
};

template <typename Tp, typename Tsrc> bool operator==(const const_locking_iterator_t<Tp, Tsrc> & a,
													  const locking_iterator_t <Tp, Tsrc> & b)
{
	return a.m_src == b.m_src;
}

template <typename Tp, typename Tsrc> bool operator==(const locking_iterator_t<Tp, Tsrc> & a,
													  const const_locking_iterator_t <Tp, Tsrc> & b)
{
	return a.m_src == b.m_src;
}

template <typename Tp, typename Tsrc> bool operator!=(const const_locking_iterator_t<Tp, Tsrc> & a,
													  const locking_iterator_t <Tp, Tsrc> & b)
{
	return a.m_src != b.m_src;
}

template <typename Tp, typename Tsrc> bool operator!=(const locking_iterator_t<Tp, Tsrc> & a,
													  const const_locking_iterator_t <Tp, Tsrc> & b)
{
	return a.m_src != b.m_src;
}



//template <typename Key, typename Tp, typename Compare = std::less<Key>, typename Alloc = std::allocator< std::pair<const Key, Tp> > >
class KVS_map : private I_KVS_nodeproxy {
private:
	typedef std::string Key;
	typedef std::string Tp;
	
	typedef std::map< Key , Tp > impl_map_t;
	
	typedef typename impl_map_t::iterator impl_iterator_t;
	typedef boost::recursive_mutex::scoped_lock lock_t;
	
	
public:
	typedef Key key_type;
	typedef Tp data_type;
	typedef std::pair<const key_type, data_type> value_type;
	typedef value_type& reference; 
	typedef const value_type& const_reference; 
	
	typedef locking_iterator_t<value_type, impl_iterator_t> iterator;
	typedef const_locking_iterator_t<value_type, impl_iterator_t> const_iterator;
	
	
	KVS_map(sp_I_KVS_node noderef): I_KVS_nodeproxy(noderef), m_cache_invalidate(false)
	{
	
	}
	iterator begin()
	{
		lock_t lock(m_mutex);
		impl_iterator_t begin_it = cache.begin();
		return iterator(m_mutex, begin_it);
	}
	
	iterator end()
	{
		lock_t lock(m_mutex);
		impl_iterator_t end_it = cache.end();
		return iterator(m_mutex, end_it);
	}
	const_iterator begin() const
	{
		lock_t lock(m_mutex);
		boost::recursive_mutex & m = m_mutex;
		return const_iterator(m, cache.end());
	}
	const_iterator end() const
	{
		lock_t lock(m_mutex);
		return const_iterator(m_mutex, cache.end());
	}

	data_type & operator[](const key_type & k)
	{
		lock_t lock(m_mutex);
		return cache.operator[](k);
	}

	const_iterator find(const key_type& __x) const
	{
		lock_t lock(m_mutex);
		return cache.find(__x);
	}

	iterator find(const key_type& __x)
	{
		lock_t lock(m_mutex);
		return cache.find(__x);
	}

	size_t count(const key_type& __x) const
	{
		return find(__x ) == end() ? 0 : 1; 
	}

	iterator lower_bound(const key_type& __k)
	{
		lock_t lock(m_mutex);
		return cache.lower_bound(__k);
	}

	const_iterator lower_bound(const key_type& __k) const
	{
		lock_t lock(m_mutex);
		return cache.lower_bound(__k);
	}

	iterator upper_bound(const key_type& __k)
	{
		lock_t lock(m_mutex);
		return cache.upper_bound(__k);
	}

	const_iterator upper_bound(const key_type& __k) const
	{
		lock_t lock(m_mutex);
		return cache.upper_bound(__k);
	}

private:
	
	// Override me in the client if notifications are needed
	KVS_NODEPROXY_NOTIFYCHANGES(path)
	{
		cache_invalidate = true;
	}
	
	void check_cache_inval()
	{
		if (cache_invalidate)
		{
			lock_t lock(m_mutex);
			cache = impl_map_t()
		}
	}
	
	impl_map_t cache;
	bool cache_invalidate;
	boost::recursive_mutex m_mutex;
};


#endif