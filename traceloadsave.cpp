#include "trace.h"
#include "xmlbzip2_fileio.h"
#include "memlocdata.h"

#include <libxml/tree.h>

#include "memsegment.h"

#include "base64.h"

// TODO: remove this
#include "arch/arm/arm.h"

#define buf_len 256
#if 0

void Trace::write_file(const char * filename)
{
}

Trace * Trace::load_project_file(const char * filename)
{
	return NULL;
}	

#if 0
static void create_hex_node(u32 data, char * name, xmlNodePtr parent)
{
	xmlChar buf[buf_len];
	xmlStrPrintf(buf, buf_len, BAD_CAST "%x", data);
	xmlNewChild(parent, NULL, BAD_CAST name, buf);
}

static void serialize_Instruction(Instruction * instr, xmlNodePtr parent)
{
	if (!instr)
		return;
	
	xmlNodePtr instr_n = xmlNewChild(parent, NULL, BAD_CAST "instruction", NULL);
	
	create_hex_node(instr->get_addr(), "addr", instr_n);
	create_hex_node(instr->get_pcflags(), "pcflags", instr_n);
	create_hex_node(instr->get_direct_jump_addr(), "dirdest", instr_n);
	create_hex_node(instr->get_opcode(), "opcode", instr_n);
}

static void serialize_symbol(Symbol * s, xmlNodePtr parent)
{
	if (!s)
		return;
	
	xmlNodePtr sym_n = xmlNewChild(parent, NULL, BAD_CAST "symbol", NULL);
	
	create_hex_node(s->get_addr(), "addr", sym_n);
	xmlNewChild(sym_n, NULL, BAD_CAST "name", BAD_CAST s->get_name().c_str());
}

static void serialize_xref(Xref * x, xmlNodePtr parent)
{	
	xmlNodePtr xref_n = xmlNewChild(parent, NULL, BAD_CAST "xref", NULL);
	
	create_hex_node(x->get_dst_addr(), "dstaddr", xref_n);
	create_hex_node(x->get_src_addr(), "srcaddr", xref_n);
	create_hex_node(x->get_type(), "type", xref_n);
}

static void serialize_memsegment(MemSegment * msg, xmlNodePtr memories_n)
{
	xmlNodePtr memory_n = xmlNewChild(memories_n, NULL, BAD_CAST "memory", NULL);
	create_hex_node(msg->get_start(), "base", memory_n);
	
	char * base64_code;
	size_t outlen = base64_encode_alloc ((const char*)msg->data_ptr(), msg->get_length(), &base64_code);
	if (base64_code == NULL)
	{
		fprintf(stderr, "err encoding memory to base64\n");
		return;
	}
	xmlNewChild(memory_n, NULL, BAD_CAST "data", BAD_CAST base64_code);	
	free(base64_code);
	
	
}
void Trace::write_file(const char * filename)
{
	xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, node = NULL, memories_n, instructions_n, symbols_n, xrefs_n;

	LIBXML_TEST_VERSION;
	
	doc = xmlNewDoc( BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "atdoc");
    xmlDocSetRootElement(doc, root_node);
	
	memories_n =		xmlNewChild(root_node, NULL, BAD_CAST "memblocks", NULL);
	instructions_n =xmlNewChild(root_node, NULL, BAD_CAST "instructions", NULL);
	symbols_n = xmlNewChild(root_node, NULL, BAD_CAST "symbols", NULL);
	xrefs_n = xmlNewChild(root_node, NULL, BAD_CAST "xrefs", NULL);
	
	for (memseglist_ci i = m_mem_segments.begin(); i != m_mem_segments.end(); i++)
		serialize_memsegment(*i, memories_n);
	
	for (memloclist_i i = m_memdata.begin(); i!=m_memdata.end(); i++)
		serialize_Instruction(dynamic_cast<Instruction *>((*i).second), instructions_n);
	
	for (SymbolList::symaddr_ci i = m_symlist.begin_addr(); i!=m_symlist.end_addr(); i++)
		serialize_symbol(*i, symbols_n);
	
	for (memloclist_i i = m_memdata.begin(); i!=m_memdata.end(); i++)
	{
		MemlocData * instr = (*i).second;
		if (!instr || !instr->has_xrefs_from())
			continue;
		
		for (xref_map_ci j = instr->begin_xref_from(); j != instr->end_xref_from(); j++)
			serialize_xref((*j).second, xrefs_n);
		
	}
	
	XMLBZIP2_writer writer;
	
	if (!writer.xmlWriteDoc(doc, filename))
		printf("Failed to save!\n");
	
    xmlFreeDoc(doc);
    xmlCleanupParser();
	
}

/********************************** Reading ***************/
static xmlNodePtr findChild(xmlNodePtr parent, char * name)
{
	xmlNodePtr node;
	
	for(node = parent->children; node != NULL; node = node->next) {
		
		if (xmlStrcmp(node->name, BAD_CAST name) == 0)
			return node;
	}
	return NULL;
}


Trace * Trace::load_project_file(const char * filename)
{

	XMLBZIP2_reader reader;
	
	xmlDocPtr doc = reader.xmlReadDoc(filename);
	
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filename);
		return NULL;
    }
	
	// Todo - make it actually load the architecture on demand
	Trace * t = new Trace(new ARMArchitecture());
	xmlNodePtr root_element = xmlDocGetRootElement(doc);
	
	xmlNodePtr memories_n = findChild(root_element, "memblocks");
	if (!memories_n)
	{
		printf("could not find memblocks section, aborting!\n");
		return NULL;
	}
		
	xmlNodePtr memory_n = findChild(memories_n, "memory");
	if (!memory_n)
	{
		printf("could not load memory, aborting!\n");
		return NULL;
	}
	

	xmlNodePtr memory_base_n = findChild(memory_n, "base");
	if (!memory_base_n)
	{
		printf("could not find memory base address, aborting!\n");
		return NULL;
	}
	address_t base_addr = strtoul((char *) xmlNodeGetContent(memory_base_n), NULL, 16);
	
	
	xmlNodePtr memory_data_n = findChild(memory_n, "data");
	if (!memory_n)
	{
		printf("could not find memory data, aborting!\n");
		return NULL;
	}
	char * base64_content = (char*)xmlNodeGetContent(memory_data_n);
	int base64_len = strlen(base64_content);
	
	char * code_buf;
	size_t code_len;
	
	bool ok = base64_decode_alloc (base64_content, base64_len, &code_buf, &code_len);
	xmlFree(base64_content);
	if (!ok)
	{
		printf("could not parse memory data, aborting!\n");
		return NULL;
	}
	MemSegment * m = new MemSegment(base_addr, code_len, code_buf);
	t->addSegment(m);
	
	free(code_buf);
	xmlNodePtr instructions_n = findChild(root_element,  "instructions");
	
	if (instructions_n)
		for(xmlNodePtr node_a = instructions_n->children; node_a != NULL; node_a = node_a->next) {
			u32 addr;
			u32 opcode;
			
			bool good = false;
			bool opcodeset = false;
			//printf("node %s\n", node_a->name);
			
			for(xmlNodePtr node_b = node_a->children; node_b != NULL; node_b = node_b->next) {
				char * content = (char *) xmlNodeGetContent(node_b);
				if (!content)
					continue;
				
				if (xmlStrcmp(node_b->name, BAD_CAST "addr") == 0)
				{
					good = true;
					addr = strtoul(content, NULL, 16);
				}
				else if (xmlStrcmp(node_b->name, BAD_CAST "opcode") == 0)
				{
					opcode = strtoul(content, NULL, 16);
					opcodeset = true;
				}
				xmlFree(content);
			}
			if (!opcodeset)
				opcode = t->ldw(addr);
			
			if (good)
			{
				// fixme
				Instruction * inst = t->m_arch->create_instruction(t, addr);//new Instruction(t, addr, opcode);
				
				t->insert_memlocd(inst);
			} else {
				printf("Instruction was missing addr!\n");
			}
		}
	else
		printf("warning, no Instruction content found for xml doc\n");
	
	xmlNodePtr symbols_n = findChild(root_element, "symbols");
	if (symbols_n)
	{
		for(xmlNodePtr node_a = symbols_n->children; node_a != NULL; node_a = node_a->next) {
			std::string name;
			u32 addr;
			
			bool goodname = false;
			bool goodaddr = false;
			
			for(xmlNodePtr node_b = node_a->children; node_b != NULL; node_b = node_b->next) {
				char * content = (char *) xmlNodeGetContent(node_b);
				if (!content)
					continue;
				
				if (xmlStrcmp(node_b->name, BAD_CAST "addr") == 0)
				{
					goodaddr = true;
					addr = strtoul(content, NULL, 16);
				}
				else if (xmlStrcmp(node_b->name, BAD_CAST "name") == 0)
				{
					name = content;
					goodname = true;
				}
	
				xmlFree(content);
			}

			if (goodname && goodaddr)
				t->create_sym(name.c_str(), addr);
			else
				printf("Symbol did not load %d %d\n", goodname, goodaddr);
		}
	} else {
		printf("Warning - no symbol section found in saved file!\n");
	}
	
	xmlNodePtr xrefs_n = findChild(root_element, "xrefs");
	if (xrefs_n)
	{
		for(xmlNodePtr node_a = xrefs_n->children; node_a != NULL; node_a = node_a->next) {
			u32 srcaddr;
			u32 dstaddr;
			u32 type;
			
			bool goodsrc = false;
			bool gooddst = false;
			bool goodtype = false;
			
			for(xmlNodePtr node_b = node_a->children; node_b != NULL; node_b = node_b->next) {
				char * content = (char *) xmlNodeGetContent(node_b);
				if (!content)
					continue;
				
				if (xmlStrcmp(node_b->name, BAD_CAST "srcaddr") == 0)
				{
					goodsrc = true;
					srcaddr = strtoul(content, NULL, 16);
				} else if (xmlStrcmp(node_b->name, BAD_CAST "dstaddr") == 0)
				{
					gooddst = true;
					dstaddr = strtoul(content, NULL, 16);
				} else if (xmlStrcmp(node_b->name, BAD_CAST "type") == 0)
				{
					goodtype = true;
					type = strtoul(content, NULL, 16);
				}
	
				xmlFree(content);
			}
			
			if (goodsrc && gooddst && goodtype)
				t->create_xref(srcaddr, dstaddr, type);
			else
				printf("Xref did not load %d %d %d\n", goodsrc, gooddst, goodtype);
		}
	} else {
		printf("Warning - no xref section found in saved file!\n");
	}
	
	
    xmlFreeDoc(doc);
	
	return t;
}
#endif
#endif
