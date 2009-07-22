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

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <list>
#include <string>

#define EXCEPTION_BUFSIZ 100

class Exception
{
 public:
	Exception(const char *message);
	virtual ~Exception();
	const char *getMessage() const;
	void printStackTrace() const;
	const std::list<std::string> &getStackTrace() const;

 private:
	char *message;
	std::list<std::string> stackTrace;
	void *buffer[EXCEPTION_BUFSIZ];
};

#endif
