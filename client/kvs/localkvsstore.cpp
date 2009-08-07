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


LocalKVSStore::LocalKVSStore() : m_root("")
{
	
}
/*
const std::string LocalKVSStore::get(const std::string & key)
{
	// Not an absolute path?
	if (key[0] != KVS_PATH_CHARACTER)
		return;
	
	
	int pos = 1;
	int len = key.size();
	int sep;
	bool done = false;
	while (!done)
	{
		sep = key.find(KVS_PATH_SEPARATOR, pos)
		if (sep == -1)
	}
}

void LocalKVSStore::set(const std::string & key, const std::string & value)
{
	
}*/
LocalKVSStore::~LocalKVSStore()
{
	
}