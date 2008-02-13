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

