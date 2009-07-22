/*
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

#include <string.h>
#include "comment.h"

Comment::Comment(std::string cmt, address_t addr)
{
	set_address(addr);
	set_comment(cmt);
}

void Comment::set_address(address_t addr)
{
	address = addr;
}

const address_t Comment::get_address() const
{
	return address;
}

void Comment::set_comment(std::string cmt)
{
	comment = cmt;

	const char *ptr = cmt.c_str();
	lines = 0;

	while ((ptr = strchr(ptr, '\n')))
		++ptr, ++lines;
}

const std::string Comment::get_comment() const
{
	return comment;
}

const int Comment::get_lines() const
{
	return lines;
}


CommentList::CommentList()
{}

Comment *CommentList::set_comment(address_t addr, std::string comment)
{
	commentlist_t::iterator it = commentmap.find(addr);
	if (it != commentmap.end())
		delete (*it).second;
	return commentmap[addr] = new Comment(comment, addr);
}

Comment *CommentList::get_comment(address_t addr) const
{
	commentlist_t::const_iterator it = commentmap.find(addr);
	if (it != commentmap.end())
		return (*it).second;
	return NULL;
}
