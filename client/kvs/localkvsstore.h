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

#ifndef _LOCALKVSSTORE_H_
#define _LOCALKVSSTORE_H_

#include "i_kvs.h"
#include <boost/shared_ptr.hpp>

class LocalKVSStore;
class LocalKVSNode;
typedef boost::shared_ptr<LocalKVSNode> sp_LocalKVSNode;
typedef boost::shared_ptr<LocalKVSStore> sp_LocalKVSStore;

class LocalKVSStore : public I_KVS {
public:
	LocalKVSStore();
	virtual const std::string & getPathValue(const std::string & key) const;
	virtual sp_I_KVS_node setPathValue(const std::string & key, const std::string & value);

	virtual sp_I_KVS_attributes createDanglingAttributes();
	virtual sp_I_KVS_node getPathNode(const std::string & key) const;
	
	virtual sp_I_KVS_node overlayNode(const std::string & key, const std::string & value = "", sp_I_KVS_attributes attribs = sp_I_KVS_attributes());
	virtual sp_I_KVS_node overwriteNode(const std::string & key, const std::string & value = "", sp_I_KVS_attributes attribs = sp_I_KVS_attributes());

	bool serializeTo(const std::string & output_filename) const;
	bool overwriteContentsFrom(const std::string  & output_filename);
	static sp_LocalKVSStore createFromFile(const std::string & input_filename);
	
	virtual ~LocalKVSStore();
private:
	sp_LocalKVSNode m_root;
};

#endif