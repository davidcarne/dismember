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

#include "xmlbzip2_fileio.h"

#include <libxml/xmlsave.h>
#include <libxml/parser.h>


int XMLBZIP2_writer::bz2xmlOutputWriteCallback(void * context, const char * buffer, int len)
{
	XMLBZIP2_writer * writer = (XMLBZIP2_writer *)(context);
	 
	int bzerror;
	BZ2_bzWrite ( &bzerror, writer->m_bz_output_file, (void*)buffer, len );
	if (bzerror == BZ_IO_ERROR) { 
		//printf("BZ2 Write error\n");
		BZ2_bzWriteClose ( &bzerror,(BZFILE*)context,0,NULL, NULL );
		return -1;
	}
	return len;
}
 
int XMLBZIP2_writer::bz2xmlOutputCloseCallback(void * context)
{
	XMLBZIP2_writer * writer = (XMLBZIP2_writer *)(context);
	 
	int bzerror;
	 
	BZ2_bzWriteClose ( &bzerror, writer->m_bz_output_file, 0, NULL, NULL );
	if (bzerror == BZ_IO_ERROR) {
		return -1;
	}
	return 0;
}
 

bool XMLBZIP2_writer::xmlWriteDoc(xmlDocPtr doc, std::string filename)
{
	xmlSaveCtxtPtr ctxt;
	
	m_output_file = fopen(filename.c_str(), "w");
	int     bzerror;
	
	m_bz_output_file = BZ2_bzWriteOpen( &bzerror, m_output_file, 9, 0, 0);
	
	if (bzerror != BZ_OK) {
		BZ2_bzWriteClose (&bzerror,m_bz_output_file,0,NULL, NULL );
		return false;
	}
	
	ctxt = xmlSaveToIO(XMLBZIP2_writer::bz2xmlOutputWriteCallback, XMLBZIP2_writer::bz2xmlOutputCloseCallback, this,"UTF-8",0);
	
	if (xmlSaveDoc(ctxt, doc) == -1)
		 return false;
	 
	if (xmlSaveClose(ctxt) == -1)
		return false;
	
	fclose(m_output_file);
	
	return true;
}


/**********************************************************************************************
 *																							  *
 *								XMLBZIP2_reader												  *
 *																							  *
 **********************************************************************************************/

int	XMLBZIP2_reader::bz2xmlInputReadCallback(void * context, char * buffer, int len)
{
	XMLBZIP2_reader * reader = (XMLBZIP2_reader *)(context);
	
	if (!reader)
		return -1;
	
	int bzerror;
	int lenread = BZ2_bzRead (&bzerror, reader->m_bz_input_file, buffer, len );
	
	if (bzerror == BZ_IO_ERROR)
	{
		reader->m_errorcode = ERR_IOERROR;
		return -1;
	}
	
	return lenread;
}

xmlDocPtr XMLBZIP2_reader::xmlReadDoc(std::string filename)
{
	xmlDocPtr doc = NULL;
	
	m_input_file = fopen(filename.c_str(), "r");
	if (!m_input_file)
	{
		m_errorcode = ERR_OPEN;
		return NULL;
	}
	
	int bzerror;
	
	m_bz_input_file = BZ2_bzReadOpen( &bzerror, m_input_file, 0, 0, NULL, 0);
	if (bzerror != BZ_OK || !m_bz_input_file)
	{
		m_errorcode = ERR_OPEN;
		return NULL;
	}
	
	doc = xmlReadIO	(XMLBZIP2_reader::bz2xmlInputReadCallback, NULL, 
					 this, 
					 filename.c_str(), 
					 NULL, 
					 0);
	
	BZ2_bzReadClose(&bzerror, m_bz_input_file);
	fclose(m_input_file);
	
	return doc;
}

