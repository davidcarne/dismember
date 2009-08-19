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

#ifndef _I_COMMENT_H_
#define _I_COMMENT_H_
#include "types.h"
#include <string>
#include <map>

/**
 * \brief Representation of a [possibly multiline] comment
 */
class I_Comment
{
 public:
	/**
	 * \brief get the string contents of the comment
	 * @return the textual contents of the comment
	 */
	const virtual std::string get_comment() const = 0;
	
	/**
	 * \brief get the address at which the comment is set
	 * @return the address of the comment
	 */
	const virtual address_t get_address() const = 0;
	
	/**
	 * \brief get the number of newlines in the comment
	 * @return the number of newlines in the comment
	 */
	const virtual int get_lines() const = 0;
	
	virtual ~I_Comment() {};
};

#endif
