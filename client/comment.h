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

#ifndef _COMMENT_H_
#define _COMMENT_H_
#include "types.h"
#include <string>
#include <map>

/**
 * \brief Representation of a [possibly multiline] comment
 */
class Comment
{
 public:
	/**
	 * \brief get the string contents of the comment
	 * @return the textual contents of the comment
	 */
	const std::string get_comment() const;
	
	/**
	 * \brief get the address at which the comment is set
	 * @return the address of the comment
	 */
	const address_t get_address() const;
	
	/**
	 * \brief get the number of newlines in the comment
	 * @return the number of newlines in the comment
	 */
	const int get_lines() const;



 protected:
	/**
	 * Constructor for a new comment 
	 */
	Comment(std::string comment, address_t addr);

 private:
	friend class CommentList;
	std::string comment;

	 void set_comment(std::string);
	 void set_address(address_t addr);
	 
	address_t address;
	int lines;
};

/**
 * Comment list - simple map wrapper
 */
class CommentList
{
 public:
	CommentList();

	/**
	 * set a new comment, or erase an existing one.
	 * @param addr Address at which to set the comment.
	 * @param comment string to set as the comment, or "" to erase the comment
	 * @return returns the pointer of the new comment, or NULL if none was created
	 */
	Comment *set_comment(address_t addr, std::string comment);
	
	/**
	 * get an existing comment
	 * @param addr the address to lookup
	 * @return the address of the comment, or NULL if none was found
	 */
	Comment *get_comment(address_t addr) const;

 private:
	typedef std::map<address_t, Comment *, address_t::less> commentlist_t;
	commentlist_t commentmap;
};

#endif
