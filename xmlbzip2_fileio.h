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

#ifndef _XMLBZIP2_FILEIO_H_
#define _XMLBZIP2_FILEIO_H_

#include <libxml/tree.h>
#include <stdlib.h>
#include <bzlib.h>
#include <string>

class XMLBZIP2_writer {
public:
	bool xmlWriteDoc(xmlDocPtr doc, std::string filename);
	std::string getLastError();
private:
	
	static int bz2xmlOutputWriteCallback(void * context, const char * buffer, int len);
	static int bz2xmlOutputCloseCallback(void * context);
	
	enum {
		ERR_NONE
	} m_errorcode;
	
	
	FILE * m_output_file;
	BZFILE * m_bz_output_file;
};


class XMLBZIP2_reader {
public:
	xmlDocPtr xmlReadDoc(std::string filename);
	std::string getLastError();
private:
	
	static int bz2xmlInputReadCallback(void * context, char * buffer, int len);
	
	FILE * m_input_file;
	BZFILE * m_bz_input_file;
	
	enum {
		ERR_NONE,
		ERR_OPEN,
		ERR_IOERROR,
		ERR_PARSE
	} m_errorcode;
	
	std::string m_error_text;
};

#endif

