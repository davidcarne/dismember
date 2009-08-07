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

typedef  std::hash_map<const std::string *, struct KVS_Node *> KVS_children;

class LocalKVSNode : I_KVS_node{
public:
	
	LocalKVSNode(const std::string & key) : m_key(key) {
		
	};
private:
	const std::string m_key;
	KVS_children m_children;
	std::string m_value;
};


#endif