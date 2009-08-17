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

#include "iostream"

class LocalKVSNode;

//#define KVS_ALLOCATION_DEBUG

#ifdef KVS_ALLOCATION_DEBUG
#define KVS_ALLOCATION_PRINT( v) { std::cout << "DEBUG: " << v << std::endl; }
#else
#define KVS_ALLOCATION_PRINT(...)
#endif
extern const std::string LKVS_EMPTY_NODE;


typedef boost::weak_ptr<LocalKVSNode> wp_LocalKVSNode;
typedef boost::shared_ptr<LocalKVSNode> sp_LocalKVSNode;

typedef boost::weak_ptr<const LocalKVSNode> wp_c_LocalKVSNode;
typedef boost::shared_ptr<const LocalKVSNode> sp_c_LocalKVSNode;

typedef  std::hash_map<const std::string, sp_LocalKVSNode> LocalKVS_children;
typedef  std::hash_map<const std::string, std::string> LocalKVS_attributes;



class LocalKVSAttributes_transferrableFacade : public I_KVS_attributes {
	class localmap : public I_KVS_attributes {
		virtual const std::string & getAttrib(const std::string & key) const {
			LocalKVS_attributes::const_iterator it = m_lkva.find(key);
			if (it == m_lkva.end())
				return LKVS_EMPTY_NODE;
			return (*it).second;
		};
		
		virtual void setAttrib(const std::string & key, const std::string & value) {
			m_lkva[key] = value;
		};
		
		
		virtual bool bound() const {
			return false;
		};
	private:
		
		LocalKVS_attributes m_lkva;
		
		friend class LocalKVSNode;
		friend class LocalKVSAttributes_transferrableFacade;
	};

	
	typedef boost::shared_ptr<localmap> sp_localmap;
public:
	LocalKVSAttributes_transferrableFacade() : pimpl(new localmap()){
		
	}
	virtual bool bound() const {
		return pimpl->bound();
	};
	
	virtual const std::string & getAttrib(const std::string & key) const
	{
		return pimpl->getAttrib(key);
	};
	virtual void setAttrib(const std::string & key, const std::string & value)
	{
		pimpl->setAttrib(key, value);
	};
	

private:
	void changeImplementation(sp_I_KVS_attributes newimpl){
		pimpl = newimpl;
	}
	
	sp_I_KVS_attributes pimpl;
	
	friend class LocalKVSNode;
};

typedef boost::shared_ptr<LocalKVSAttributes_transferrableFacade> sp_LocalKVSAttributes_transferrableFacade;

class LocalKVSNode : public I_KVS_node, public I_KVS_attributes{
public:
	
	virtual void overlayAttributes(const sp_I_KVS_attributes attsrc);
	
	virtual void overwriteAttributes(const sp_I_KVS_attributes attsrc);
	
	virtual const std::string & getAttrib(const std::string & key) const {
		sp_LocalKVSNode child = findChild(key);
		if (!child)
			return LKVS_EMPTY_NODE;
		
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
	
	
	virtual bool bound() const {
		return true;
	};
	
	virtual const std::string & getKey() const
	{
		return m_key;
	}
	
	virtual std::string getPath() const
	{
		std::string path = m_key;
		sp_c_LocalKVSNode nptr = m_me.lock();
		
		// sp_LocalKVSNode constructor creates scope-locked conversion to shared_ptr, so no concurrency issues
		while (nptr = nptr->m_parent.lock())
		{
			path = nptr->getKey() + "/" + path;
		}
		return path;
	}
	
	
	virtual std::string getValue() const
	{
		return m_value;
	}
	
	virtual void setValue(const std::string & value)
	{
		m_value = value;
	}
	static sp_LocalKVSNode createKVSNode(const std::string & key, const wp_LocalKVSNode & parent = wp_LocalKVSNode())
	{
		sp_LocalKVSNode node = sp_LocalKVSNode(new LocalKVSNode(key, parent));
		node->m_me = node;
		return node;
	}
	
	virtual ~LocalKVSNode()
	{
		KVS_ALLOCATION_PRINT("Deleting node: " << m_key);
	};
	
private:
	LocalKVSNode(const std::string & key, const wp_LocalKVSNode & parent = wp_LocalKVSNode()) : m_key(key), m_parent(parent) {
		
		KVS_ALLOCATION_PRINT("Creating node: " << m_key);
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