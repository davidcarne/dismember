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

#ifndef _I_KVS_PROXIES_H_
#define _I_KVS_PROXIES_H_

#include "i_kvs.h"
#include <boost/bind.hpp>

// Privately inherit from this, and use BRING_IN_KVS
class I_KVS_attribproxy {
public:
	/*
	 * - HAS TEST
	 */
	const std::string & getAttrib(const std::string & key) const {
		return m_attribref->getAttrib(key);
	};
	
	/*
	 * - HAS TEST
	 */
	void setAttrib(const std::string & key, const std::string & value) {
		m_attribref->setAttrib(key, value);
	};
protected:
	I_KVS_attribproxy(sp_I_KVS_attributes attribref) : m_attribref(attribref) {
		
	};
	
	virtual ~I_KVS_attribproxy() {
		
	};
private:
	sp_I_KVS_attributes m_attribref;
};

#define BRING_IN_KVS_ATTRIBS \
using I_KVS_attribproxy::getAttrib; \
using I_KVS_attribproxy::setAttrib;

class kvs_fake_iterator {
public:
	typedef std::forward_iterator_tag iterator_category;
	typedef sp_I_KVS_node value_type;
	typedef ptrdiff_t difference_type;
	typedef sp_I_KVS_node * pointer;
	typedef sp_I_KVS_node & reference;
	
	
	sp_I_KVS_node operator*()
	{
		return sp_I_KVS_node();
	}
	kvs_fake_iterator operator++()
	{
		return kvs_fake_iterator();
	}
};

bool operator==(const kvs_fake_iterator & a, const kvs_fake_iterator &b);

#define KVS_NODEPROXY_NOTIFYCHANGES(name) virtual void notifyChanges(const std::string & name)


// Privately inherit from this, and use BRING_IN_KVS_NODE
class I_KVS_nodeproxy {
public:
	
protected:
	I_KVS_nodeproxy(sp_I_KVS_node attribref) : m_attribref(attribref) {
		
		cb_kvs_tree_changed fbo = boost::bind(&I_KVS_nodeproxy::rcvChangeNotify, this, _1);
		if (attribref)
			m_subscription = attribref->subscribeChanges(fbo);
	};
	
	virtual ~I_KVS_nodeproxy() {
		
	};
	
	/* Node reference has been deleted from the tree */
	// HAS TEST
	bool isDeleted()
	{
		return !m_attribref.lock();
	};
	
	
	/*
	 * - HAS TEST
	 */
	const std::string & getKey() const
	{
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return kvs_empty_string;
		return n->getKey();
	};
	
	
	/*
	 * - HAS TEST
	 */
	std::string getPath() const
	{
		
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return kvs_empty_string;
		return n->getPath();
	};
	
	
	/*
	 * - HAS TEST
	 */
	std::string getPathValue(const std::string & path) const
	{
		
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return kvs_empty_string;
		
		return n->getPathValue(path);
	};
	
	
	/*
	 * - HAS TEST
	 */
	void setPathValue(const std::string & path, const std::string & value) const
	{
		
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return;
		
		n->setPathValue(path, value);
	};
	
	/*
	 * - HAS TEST
	 */
	std::string getValue() const
	{		
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return kvs_empty_string;
		return n->getValue();
	};
	
	/*
	 * - HAS TEST
	 */
	void setValue(const std::string & value)
	{
		sp_I_KVS_node n(m_attribref.lock());
		
		if (!n)
			return;
		
		n->setValue(value);
	};
	
	/*
	 * - HAS TEST
	 */
	kvs_node_child_ci beginChildren() const
	{
		kvs_node_child_ci i;
		
		sp_I_KVS_node n(m_attribref.lock());
		if (!n)
			i = kvs_fake_iterator();
		else
			i = n->beginChildren();
		return i;
		
	};
	/*
	 * - HAS TEST
	 */
	kvs_node_child_ci endChildren() const
	{
		kvs_node_child_ci i;
		
		sp_I_KVS_node n(m_attribref.lock());
		if (!n)
			i = kvs_fake_iterator();
		else
			i = n->endChildren();
		return i;
	};
	
	/*
	 * - HAS TEST
	 */
	uint32_t getChildCount() const
	{
		sp_I_KVS_node n(m_attribref.lock());
		if (!n)
			return 0;
		return n->getChildCount();
	};
	
	// Override me in the client if notifications are needed
	KVS_NODEPROXY_NOTIFYCHANGES(path)
	{
	}
	
private:
	// Indirection; otherwise boost::function points to this class ignoring virtual
	void rcvChangeNotify(const std::string & path)
	{
		notifyChanges(path);
	}
	
	sp_subscription m_subscription;
	wp_I_KVS_node m_attribref;
};

#define BRING_IN_KVS_NODE \
using I_KVS_nodeproxy::isDeleted; \
using I_KVS_nodeproxy::getKey; \
using I_KVS_nodeproxy::getPath; \
using I_KVS_nodeproxy::getPathValue; \
using I_KVS_nodeproxy::setPathValue; \
using I_KVS_nodeproxy::getValue; \
using I_KVS_nodeproxy::setValue; \
using I_KVS_nodeproxy::beginChildren; \
using I_KVS_nodeproxy::endChildren; \
using I_KVS_nodeproxy::getChildCount;

#endif