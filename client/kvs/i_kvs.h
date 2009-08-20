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

#ifndef _I_KVS_H_
#define _I_KVS_H_

#define KVS_PATH_SEPARATOR "/"
#define KVS_PATH_CHARACTER '/'

// quick notation for KVS paths, aka
// _$ "path" _$ "to" _$ "the" _$ "stuff"
// Don't use the "/". I may change that to a nonprinting character in the future
#define _$ KVS_PATH_SEPARATOR

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "any_iterator.hpp"

extern const std::string kvs_empty_string;

// View of a node that just represents its attributes [aka, children without children]
// Could be thought of as a leaf node; but doesn't contain name
class I_KVS_attributes {
public:
	/*
	 * - HAS TEST
	 */
	virtual const std::string & getAttrib(const std::string & key) const = 0;
	
	
	/*
	 * - HAS TEST
	 */
	virtual void setAttrib(const std::string & key, const std::string & value) = 0;
	
	virtual bool bound() const = 0;
	virtual ~I_KVS_attributes();
	
};

typedef boost::shared_ptr<I_KVS_attributes> sp_I_KVS_attributes;
typedef boost::shared_ptr<I_KVS_attributes> wp_I_KVS_attributes;

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





class I_KVS_node;

typedef boost::shared_ptr<I_KVS_node> sp_I_KVS_node;
typedef boost::weak_ptr<I_KVS_node> wp_I_KVS_node;

typedef IteratorTypeErasure::any_iterator<sp_I_KVS_node, std::forward_iterator_tag, sp_I_KVS_node> kvs_node_child_ci;

typedef boost::function<void(const std::string &)> cb_kvs_tree_changed;
typedef boost::shared_ptr<cb_kvs_tree_changed> sp_subscription;
typedef boost::weak_ptr<cb_kvs_tree_changed> wp_subscription;

// View of a node that represents it + its children
class I_KVS_node {
public:
	/*
	 * - HAS Test
	 */
	virtual sp_I_KVS_attributes getAttributesReference() = 0;
	
	/* Overlay the attributes - anything in attsrc gets set in this node.
	 * all existing attributes not set in attsrc left alone
	 * attributes object will then be bound to node, and all changes thereafter in node reflected in attributes
	 * and vice versa
	 * - HAS TEST
	 */
	virtual void overlayAttributes(const sp_I_KVS_attributes attsrc) = 0;
	
	
	/* Overwrite the attributes - anything in attsrc gets set in this node.
	 * all existing attributes not set in attsrc deleted
	 * attributes object will then be bound to node, and all changes thereafter in node reflected in attributes
	 * and vice versa
	 * - HAS TEST
	 */
	virtual void overwriteAttributes(const sp_I_KVS_attributes attsrc) = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual const std::string & getKey() const = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual std::string getPath() const = 0;
	

	/* 
	 * gets value of a relative path from this node
	 * - HAS TEST
	 */
	virtual std::string getPathValue(const std::string &) const = 0;
	
	/* 
	 * sets value of a relative path from this node
	 * - HAS TEST
	 */
	virtual void setPathValue(const std::string & key, const std::string & value) = 0;

	/*
	 * - HAS TEST
	 */
	virtual std::string getValue() const = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual void setValue(const std::string & value) = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual kvs_node_child_ci beginChildren() const = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual kvs_node_child_ci endChildren() const = 0;
	
	/*
	 * - HAS TEST
	 */
	virtual uint32_t getChildCount() const = 0;
	
	
	// This subscription is only valid while the pointer is held by the subscriber.
	virtual sp_subscription subscribeChanges(const cb_kvs_tree_changed & fnobj) = 0;
	
	virtual ~I_KVS_node() = 0;
};


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

class I_KVS {
public:
	/**
	 * Create an unattached set of attributes; useful for something decoding, but not sure
	 * when / if will be attached to tree
	 * - HAS TESTS
	 */
	virtual sp_I_KVS_attributes createDanglingAttributes() = 0;
	
	/**
	 * Get the value of an absolute path (starting with /)
	 * - HAS TESTS
	 */
	virtual const std::string & getPathValue(const std::string & key) const = 0;
	
	/**
	 * Get a node reference absolute path. Returns invalid pointer if does not exist
	 * - HAS TESTS
	 */
	virtual sp_I_KVS_node getPathNode(const std::string & key) const = 0;
	
	
	/**
	 * Overlay attributes onto a node. Will create node if does not exist.
	 * - HAS TESTS
	 */
	virtual sp_I_KVS_node overlayNode(const std::string & key, const std::string & value = "", sp_I_KVS_attributes attribs = sp_I_KVS_attributes()) = 0;
	
	/**
	 * Overlay attributes onto a node. Will create node if does not exist.
	 * - Has TESTS
	 */
	virtual sp_I_KVS_node overlayNode(const std::string & key, sp_I_KVS_attributes attribs = sp_I_KVS_attributes())
	{
		return overlayNode(key, "", attribs);
	}
	
	/**
	 * Overwrite attributes onto a node. Will create node if does not exist. Will delete existing node + structure
	 * - Has TESTS
	 */
	virtual sp_I_KVS_node overwriteNode(const std::string & key, const std::string & value = "", sp_I_KVS_attributes attribs = sp_I_KVS_attributes()) = 0;
	
	/**
	 * Overwrite attributes onto a node. Will create node if does not exist. Will delete existing node + structure	
	 * - Has TESTS
	 */
	virtual sp_I_KVS_node overwriteNode(const std::string & key, sp_I_KVS_attributes attribs = sp_I_KVS_attributes())
	{
		return overwriteNode(key, "", attribs);
	}
	
	/**
	 * Get an absolute path (starting with /)
	 * - HAS TESTS
	 */
	virtual sp_I_KVS_node setPathValue(const std::string & key, const std::string & value) = 0;
	
	
	virtual ~I_KVS() = 0;
};


typedef boost::shared_ptr<I_KVS> sp_I_KVS;
typedef boost::weak_ptr<I_KVS> wp_I_KVS;
#endif