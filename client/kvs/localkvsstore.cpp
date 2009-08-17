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

#include "stdio.h"
#include <list>

LocalKVSStore::LocalKVSStore()
{
	m_root = LocalKVSNode::createKVSNode("");
}

const std::string & LocalKVSStore::getValue(const std::string & key)
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
		
		cursor = cursor->findChild(ptok);
		
		if (!cursor)
			return LKVS_EMPTY_NODE;
	}
	
	return cursor->m_value;
}

void LocalKVSStore::setValue(const std::string & key, const std::string & value)
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
		
		cursor = cursor->findChild_create(ptok);
		
		if (!cursor)
			return;
	}
	
	cursor->m_value = value;
}

sp_I_KVS_attributes LocalKVSStore::createDanglingAttributes()
{
	return sp_I_KVS_attributes(new LocalKVSAttributes());
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



bool LocalKVSStore::serializeTo(const std::string & output_filename) const
{
	std::list<sp_LocalKVSNode> ser_q;
	ser_q.push_back(m_root);
	int level = 0;
	while (ser_q.size())
	{
		sp_LocalKVSNode node = ser_q.front();
		ser_q.pop_front();
		
		for (LocalKVS_children::iterator it = node->m_children.begin();
			 it != node->m_children.end(); it++)
		{
			ser_q.push_back((*it).second);
		}
		
		if (node->getValue().length() != 0)
			std::cout << node->getPath() << "::" << node->getValue() << std::endl;
	}
}

bool LocalKVSStore::overwriteContentsFrom(const std::string  & input_filename)
{
	// Zero the root; should cause deallocation of the whole tree.
	m_root = LocalKVSNode::createKVSNode("");
}

sp_LocalKVSStore LocalKVSStore::createFromFile(const std::string & input_filename)
{
	sp_LocalKVSStore kvsp = sp_LocalKVSStore(new LocalKVSStore);
	
	if (kvsp->overwriteContentsFrom(input_filename))
		return kvsp;
	
	return sp_LocalKVSStore();
}
