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

#include "localkvsnode_private.h"
const std::string LKVS_EMPTY_NODE = "";



void LocalKVSNode::overlayAttributes(const sp_I_KVS_attributes attsrc)
{
	assert(!attsrc->bound());
	sp_LocalKVSAttributes_transferrableFacade lkaf = 
		boost::dynamic_pointer_cast<LocalKVSAttributes_transferrableFacade>(attsrc);
	
	LocalKVSAttributes_transferrableFacade::sp_localmap lka = 
		boost::dynamic_pointer_cast<LocalKVSAttributes_transferrableFacade::localmap>(lkaf->pimpl);
	
	LocalKVS_attributes::const_iterator ci = lka->m_lkva.begin();
	
	for (; ci != lka->m_lkva.end(); ci++)
		setAttrib((*ci).first, (*ci).second);
	
	sp_I_KVS_attributes newtarget = boost::dynamic_pointer_cast<I_KVS_attributes>(m_me.lock());
	lkaf->changeImplementation(newtarget);
}


void LocalKVSNode::overwriteAttributes(const sp_I_KVS_attributes attsrc)
{
	m_children = LocalKVS_children();
	overlayAttributes(attsrc);
}

sp_subscription LocalKVSNode::subscribeChanges(const cb_kvs_tree_changed & fnobj)
{
	
	sp_subscription sub = sp_subscription(new cb_kvs_tree_changed(fnobj));
	m_subs.insert(sub);
	return sub;
}
void LocalKVSNode::postChange(const std::string & path) const
{
	for (std::set<wp_subscription>::iterator i = m_subs.begin(); i!=m_subs.end(); i++)
	{
		sp_subscription subptr = (*i).lock();
		if (subptr)
		{
			cb_kvs_tree_changed fn = (*subptr.get());
			fn(path);
			
		}
	}
	
	sp_c_LocalKVSNode pptr = m_parent.lock();
	if (pptr)
		pptr->postChange(path);
}