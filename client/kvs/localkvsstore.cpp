/*	Copyright 2009 David Carne
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


#include "LocalKVSStore.h"

#include <boost/tokenizer.hpp>

LocalKVSStore::LocalKVSStore()
{
	m_root = LocalKVSNode::createKVSNode("");
}

const std::string & LocalKVSStore::getValue(const std::string & key)
{
	return "";
}

void LocalKVSStore::setValue(const std::string & key, const std::string & value)
{
}

I_KVS_attributes * LocalKVSStore::createDanglingAttributes()
{
	return NULL;
}

sp_I_KVS_node LocalKVSStore::getNode(const std::string & key)
{
	std::string mykey = key;
	assert(mykey[0] == '/');
	
	sp_LocalKVSNode cursor = m_root;
	
	typedef boost::tokenizer<boost::char_separator<char> > path_tokenizer;
	boost::char_separator<char> sep("/");
	path_tokenizer tokens(key, sep);
	
	for (path_tokenizer::iterator path_comp_iter = tokens.begin();
		 path_comp_iter != tokens.end(); path_comp_iter++)
	{
		const std::string & ptok = *path_comp_iter;
		LocalKVS_children & node_children = cursor->m_children;
		
		LocalKVS_children::iterator ci = node_children.find(ptok);
		if (ci == node_children.end())
			return sp_I_KVS_node();
		
		cursor = sp_LocalKVSNode((*ci).second);
		
		if (!cursor)
			return sp_I_KVS_node();
	}
	return cursor;
}

LocalKVSStore::~LocalKVSStore()
{
	
}