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
