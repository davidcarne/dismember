/*  Copyright 2009 David Carne
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

#ifndef _KVS_NODE_H_
#define _KVS_NODE_H_

#include "i_kvs.h"

#include "hash_map.h"

class LocalKVSNode;




typedef boost::weak_ptr<LocalKVSNode> wp_LocalKVSNode;
typedef boost::shared_ptr<LocalKVSNode> sp_LocalKVSNode;

typedef boost::weak_ptr<const LocalKVSNode> wp_c_LocalKVSNode;
typedef boost::shared_ptr<const LocalKVSNode> sp_c_LocalKVSNode;

typedef  std::hash_map<const std::string, sp_LocalKVSNode> LocalKVS_children;


class LocalKVSNode : public I_KVS_node, public I_KVS_attributes{
public:
	static const std::string EMPTY_NODE;
	
	virtual void overlayAttributes(const sp_I_KVS_attributes attsrc);
	
	virtual const std::string & getAttrib(const std::string & key) const {
		sp_LocalKVSNode child = findChild(key);
		if (!child)
			return EMPTY_NODE;
		
		return child->m_value;
	}
	
	
	virtual sp_I_KVS_attributes LocalKVSNode::getAttributesReference()
	{
		return sp_I_KVS_attributes(m_me);
	}
	
	virtual void setAttrib(const std::string & key, const std::string & value) {
		
		sp_LocalKVSNode sp_child = findChild_create(key);
		sp_child->m_value = value;
	}
	
	virtual const std::string & getKey() const
	{
		return m_key;
	}
	
	virtual std::string getPath() const
	{
		std::string path = m_key;
		sp_c_LocalKVSNode nptr = sp_c_LocalKVSNode(this);
		
		// sp_LocalKVSNode constructor creates scope-locked conversion to shared_ptr, so no concurrency issues
		while (nptr = sp_c_LocalKVSNode(nptr->m_parent))
		{
			
			path = nptr->getKey() + "/" + path;
		}
		return path;
	}
	
	static sp_LocalKVSNode createKVSNode(const std::string & key, const wp_LocalKVSNode & parent = wp_LocalKVSNode())
	{
		sp_LocalKVSNode node = sp_LocalKVSNode(new LocalKVSNode(key, parent));
		node->m_me = node;
		return node;
	}
private:
	LocalKVSNode(const std::string & key, const wp_LocalKVSNode & parent = wp_LocalKVSNode()) : m_key(key), m_parent(parent) {
		
	};
	
	sp_LocalKVSNode findChild(const std::string & key) const
	{
		LocalKVS_children::const_iterator ci = m_children.find(key);
		if (ci != m_children.end())
			return (*ci).second;
		return sp_LocalKVSNode();
	}
	sp_LocalKVSNode findChild_create(const std::string & key)
	{
		sp_LocalKVSNode sp_child = findChild(key);
		if (sp_child)
			return sp_child;
		wp_LocalKVSNode parent = m_me;
		
		sp_child = createKVSNode(key, parent);
		m_children[key] = sp_child;
		return sp_child;
		
	};
	
	wp_LocalKVSNode m_me;
	wp_LocalKVSNode m_parent;
	const std::string m_key;
	LocalKVS_children m_children;
	std::string m_value;

	friend class LocalKVSStore;
};


#endif