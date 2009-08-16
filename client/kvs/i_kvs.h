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

// View of a node that just represents its attributes [aka, children without children]
// Could be thought of as a leaf node; but doesn't contain name
class I_KVS_attributes {
public:
	virtual const std::string & getAttrib(const std::string & key) const = 0;
	virtual void setAttrib(const std::string & key, const std::string & value) = 0;
	
	virtual ~I_KVS_attributes();
	
};

typedef boost::shared_ptr<I_KVS_attributes> sp_I_KVS_attributes;

// Privately inherit from this, and use BRING_IN_KVS
class I_KVS_attribproxy : public I_KVS_attributes{
public:
	virtual const std::string & getAttrib(const std::string & key) const {
		return m_attribref->getAttrib(key);
	};
	virtual void setAttrib(const std::string & key, const std::string & value) {
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

#define BRING_IN_KVS_ATTRIBS public: \
	using I_KVS_attribproxy::getAttrib; \
	using I_KVS_attribproxy::setAttrib;

// View of a node that represents it + its children
class I_KVS_node {
public:
	virtual sp_I_KVS_attributes getAttributesReference() = 0;
	virtual void overlayAttributes(const sp_I_KVS_attributes attsrc) = 0;
	
	virtual const std::string & getKey() const = 0;
	virtual std::string getPath() const = 0;
	// TODO: Children accessors
	
	virtual ~I_KVS_node() = 0;
};

typedef boost::shared_ptr<I_KVS_node> sp_I_KVS_node;
typedef boost::weak_ptr<I_KVS_node> wp_I_KVS_node;

class I_KVS {
public:
	/**
	 * Create an unattached set of attributes; useful for something decoding, but not sure
	 * when / if will be attached to tree
	 */
	virtual I_KVS_attributes * createDanglingAttributes() = 0;
	
	/**
	 * Get the value of an absolute path (starting with /)
	 */
	virtual const std::string & getValue(const std::string & key) = 0;
	
	virtual sp_I_KVS_node getNode(const std::string & key) = 0;
	
	/**
	 * Get an absolute path (starting with /)
	 */
	virtual void setValue(const std::string & key, const std::string & value) = 0;
	
	
	virtual ~I_KVS() = 0;
};

#endif