/* 
 * Copyright (c) 2005 Joshua Oreman. All rights reserved.
 * Copyright (c) 2009 Felix Bruns. Modified and adjusted for use in dismember.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include "../loaderfactory.h"
#include "memsegment.h"

#include "elf32.h"
//#include "elf64.h"

/* Relocations. */
#define RELOC_HAS_ADDEND (1<<15)
typedef struct relocation {
	struct section    *section;
	unsigned int       offset;
	unsigned int       symbolIndex;
	unsigned int       type;
	int32_t            addend;
	struct symbol    *symbol;
	struct relocation *next;
} relocation;

/* Symbols. */
typedef struct symbol {
	const char     *name;
	unsigned int    nameIndex;
	unsigned int    value;
	unsigned int    size;
	unsigned int    type;
	unsigned int    binding;
	int16_t         sectionIndex;
	struct section *section;
} symbol;

/* Sections. */
typedef struct section {
	const char     *name;
	unsigned int    nameIndex;
	unsigned int    type;
	unsigned int    flags;
	char           *addr;
	unsigned int    offset;
	unsigned int    size;
	unsigned int    entsize;
	unsigned int    linkIndex;
	unsigned int    info;
	unsigned int    alignment;
	unsigned int    alignmask;
	struct section *link;
	struct section *relsection;
} section;

class ElfLoader : public FileLoaderMaker {
	public:
		ElfLoader();
		virtual int matchToFile(FILE * f) const;
		virtual bool loadFromFile(FILE * f, Trace * ctx);
	private:
		symbol    *symbols;
		section    *sections;
		relocation *relocations;
		char      **strtabs;
} registerElfLoader;

static uint8_t read8(FILE *fp)
{
	return fgetc(fp);
}

static uint16_t MSB_read16(FILE *fp)
{
	uint16_t u = read8(fp);
	uint16_t l = read8(fp);
	return (uint16_t)(l << 8) | u;
}

static uint16_t LSB_read16(FILE *fp)
{
	uint16_t u = read8(fp);
	uint16_t l = read8(fp);
	return (uint16_t)(u << 8) | l;
}

static uint32_t MSB_read32(FILE *fp)
{
	uint16_t u = MSB_read16(fp);
	uint16_t l = MSB_read16(fp);
	return (uint32_t)(l << 16) | u;
}

static uint32_t LSB_read32(FILE *fp)
{
	uint16_t u = LSB_read16(fp);
	uint16_t l = LSB_read16(fp);
	return (uint32_t)(u << 16) | l;
}

struct readFnTable {
	uint8_t  (*  read8)(FILE *fp);
	uint16_t (* read16)(FILE *fp);
	uint32_t (* read32)(FILE *fp);
};

struct readFnTable LSB_readFnTable = {
	read8, LSB_read16, LSB_read32
};

struct readFnTable MSB_readFnTable = {
	read8, MSB_read16, MSB_read32
};

struct readFnTable *getReadFnTable(Elf32_Ehdr *eh)
{
	int swap_endian = 0;
#ifdef _BIG_ENDIAN
	swap_endian = (eh->e_ident[EI_DATA] == ELFDATA2LSB);
#else
	// Wot? Why does this work?
	swap_endian = (eh->e_ident[EI_DATA] == ELFDATA2LSB);
#endif
	return swap_endian ? &MSB_readFnTable : &LSB_readFnTable;
}

static int readFileHeader(FILE *fp, Elf32_Ehdr *eh)
{
	if (fread(eh->e_ident, 1, EI_NIDENT, fp) != EI_NIDENT)
		return 1;
	struct readFnTable *rft = getReadFnTable(eh);
	eh->e_type      = rft->read16(fp);
	eh->e_machine   = rft->read16(fp);
	eh->e_version   = rft->read32(fp);
	eh->e_entry     = rft->read32(fp);
	eh->e_phoff     = rft->read32(fp);
	eh->e_shoff     = rft->read32(fp);
	eh->e_flags     = rft->read32(fp);
	eh->e_ehsize    = rft->read16(fp);
	eh->e_phentsize = rft->read16(fp);
	eh->e_phnum     = rft->read16(fp);
	eh->e_shentsize = rft->read16(fp);
	eh->e_shnum     = rft->read16(fp);
	eh->e_shstrndx  = rft->read16(fp);

	return 0;
}

static int readSectionHeader(FILE *fp, Elf32_Shdr *sh, struct readFnTable *rft)
{
	sh->sh_name      = rft->read32(fp);
	sh->sh_type      = rft->read32(fp);
	sh->sh_flags     = rft->read32(fp);
	sh->sh_addr      = rft->read32(fp);
	sh->sh_offset    = rft->read32(fp);
	sh->sh_size      = rft->read32(fp);
	sh->sh_link      = rft->read32(fp);
	sh->sh_info      = rft->read32(fp);
	sh->sh_addralign = rft->read32(fp);
	sh->sh_entsize   = rft->read32(fp);

	return 0;
}

static int readRelocation(FILE *fp, Elf32_Rel *r, struct readFnTable *rft)
{
	r->r_offset = rft->read32(fp);
	r->r_info   = rft->read32(fp);
	return 0;
}

static int readRelocationAddend(FILE *fp, Elf32_Rela *r, struct readFnTable *rft)
{
	r->r_offset = rft->read32(fp);
	r->r_info   = rft->read32(fp);
	r->r_addend = rft->read32(fp);
	return 0;
}


static int readSymbolEntry(FILE *fp, Elf32_Sym *st, struct readFnTable *rft)
{
	st->st_name  = rft->read32(fp);
	st->st_value = rft->read32(fp);
	st->st_size  = rft->read32(fp);
	st->st_info  = rft->read8(fp);
	st->st_other = rft->read8(fp);
	st->st_shndx = rft->read16(fp);
	return 0;
}

ElfLoader::ElfLoader() : FileLoaderMaker("ELF") {
}

int ElfLoader::matchToFile(FILE * file) const {
	Elf32_Ehdr eh;
	
	/* Read ELF header. */
	if (readFileHeader(file, &eh))
		return -1;
	
	/* Check if header is supported. */
	if(IS_ELF(eh) &&
	   eh.e_ident[EI_CLASS] == ELFCLASS32  &&
	   eh.e_type            == ET_REL      &&
	   eh.e_machine         == EM_ARM      &&
	   eh.e_shoff){
		return 100;
	}

	return -1;
}

bool ElfLoader::loadFromFile(FILE *file, Trace *trace){
	Elf32_Ehdr eh;
	Elf32_Shdr sh;
	u32      i, j;
	
	/* Read ELF header. */
	if(readFileHeader(file, &eh)) {
		fprintf(stderr, "Failed to read ELF header.\n");
		return false;
	}
	
	/* Check if file is valid. */
	if(!IS_ELF(eh)){
		fprintf(stderr, "Not an ELF file (%x %x %x %x).\n",
			eh.e_ident[EI_MAG0], eh.e_ident[EI_MAG1],
			eh.e_ident[EI_MAG2], eh.e_ident[EI_MAG3]
		);
		
		return false;
	}
	else if(eh.e_ident[EI_CLASS] != ELFCLASS32){
		fprintf(stderr, "Not a 32-bit ELF file (%d).\n", eh.e_ident[EI_CLASS]);
		
		return false;
	}
	else if(eh.e_type != ET_REL){
		fprintf(stderr, "Not an ELF relocatable (%d).\n", eh.e_ident[EI_DATA]);
		
		return false;
	}
	else if(eh.e_machine != EM_ARM) {
		fprintf(stderr, "Not an ARM ELF file (%d).\n", eh.e_machine);
		
		return false;
	}
	else if(!eh.e_shoff){
		fprintf(stderr, "No section header table.\n");
		
		return false;
	}
	
	struct readFnTable *rft = getReadFnTable(&eh);

	/* Allocate memory for sections. */
	u32  shnum          = eh.e_shnum;
	u32  shstrtabIndex  = eh.e_shstrndx;
	u32  symstrtabIndex = 0;
	u32  nsyms          = 0;
	u32  nrels          = 0;
	u32  size           = 0;
	u8  *data           = NULL;
	
	symbols     = NULL;
	sections    = (section *)calloc(shnum, sizeof(section));
	relocations = NULL;
	strtabs     = (char **)calloc(shnum, sizeof(char *));
	
	/* Fill strtabs with zero terminators. */
	for(i = 0; i < shnum; i++){
		strtabs[i] = 0;
	}
	
	/* Read section headers. */
	section *section;
	
	fseek(file, eh.e_shoff, SEEK_SET);
	
	for(section = sections, i = 0; i < shnum; i++, section++){
		if (readSectionHeader(file, &sh, rft)) {
			fprintf(stderr, "Failed to read section header.\n");
			return false;
		}
		
		section->nameIndex = sh.sh_name;
		section->type      = sh.sh_type;
		section->flags     = sh.sh_flags;
		section->addr      = (char *)sh.sh_addr;
		section->offset    = sh.sh_offset;
		section->size      = sh.sh_size;
		section->linkIndex = sh.sh_link;
		section->info      = sh.sh_info;
		section->alignment = sh.sh_addralign;
		section->alignmask = (1 << section->alignment) - 1;
		section->entsize   = sh.sh_entsize;
		
		/* Figure out memory size. */
		if(sh.sh_type == SHT_PROGBITS || sh.sh_type == SHT_NOBITS){
			size += sh.sh_size;
			
			if(size & section->alignmask){
				size = (size + section->alignmask) & ~section->alignmask;
			}
		}
	}
	
	/* Allocate memory. */
	data = (u8 *)malloc(size);
	
	if(!data){
		return false;
	}
	
	memset(data, 0, size);
	
	/* Load sections, resolve indices. */
	u32 lastoff = 0;
	
	for(section = sections, i = 0; i < shnum; i++, section++){
		if(section->linkIndex != SHN_UNDEF){
			section->link = sections + section->linkIndex;
		}
		else{
			section->link = 0;
		}
		
		switch(section->type){
			case SHT_PROGBITS: {
				if(!(section->flags & SHF_ALLOC)){
					break;
				}
				
				if(lastoff & section->alignmask){
					lastoff = (lastoff + section->alignmask) & ~section->alignmask;
				}
				
				fseek(file, section->offset, SEEK_SET);
				
				section->addr = ((char *)data) + lastoff;
				
				lastoff += section->size;
				
				if((fread(section->addr, 1, section->size, file)) != section->size){
					fprintf(stderr, "Failed to read program bits.\n");
					
					return false;
				}
				
				break;
			}
			case SHT_NOBITS: {
				if(!(section->flags & SHF_ALLOC)){
					break;
				}
				
				if(lastoff & section->alignmask){
					lastoff = (lastoff + section->alignmask) & ~section->alignmask;
				}
				
				section->addr = ((char *)data) + lastoff;
				
				lastoff += section->size;
				
				memset(section->addr, 0, section->size);
				
				break;
			}
			case SHT_STRTAB: {
				strtabs[i] = (char *)malloc(section->size);
				
				fseek(file, section->offset, SEEK_SET);
				
				if(fread(strtabs[i], 1, section->size, file) != section->size){
					fprintf(stderr, "Failed to read string table.\n");
					
					return false;
				}
				
				break;
			}
			case SHT_SYMTAB: {
				nsyms          = section->size / section->entsize;
				symstrtabIndex = section->linkIndex;
				symbols        = (symbol *)calloc(nsyms, sizeof(symbol));
				
				symbol    *symbol = symbols;
				Elf32_Sym  st;
				
				fseek(file, section->offset, SEEK_SET);
				
				for(j = 0; j < nsyms; j++, symbol++){
					if (readSymbolEntry(file, &st, rft)) {
					//if(fread(&st, 1, section->entsize, file) != section->entsize){
						fprintf(stderr, "Failed to read symbol entry.\n");
						
						return false;
					}
					
					symbol->nameIndex    = st.st_name;
					symbol->value        = st.st_value;
					symbol->size         = st.st_size;
					symbol->binding      = st.st_info >> 4;
					symbol->type         = st.st_info & 0xf;
					symbol->sectionIndex = st.st_shndx;
					symbol->section      = 0;
				}
				
				break;
			}
			case SHT_REL:
			case SHT_RELA: {
				nrels = section->size / section->entsize;
				
				relocation *reloc = relocations;
				Elf32_Rel   rel;
				Elf32_Rela  rela;
				
				if(reloc){
					while(reloc->next) reloc = reloc->next;
				}

				fseek(file, section->offset, SEEK_SET);

				for(j = 0; j < nrels; j++){
					if(section->type == SHT_REL){
						if(readRelocation(file, &rel, rft)) {
							fprintf(stderr, "Failed to read relocation entry.\n");
							
							return false;
						}
					}
					else{
						if(readRelocationAddend(file, &rela, rft)) {
							fprintf(stderr, "Failed to read relocation entry.\n");
							
							return false;
						}
					}

					if(!((sections + section->info)->flags & SHF_ALLOC)){
						continue;
					}

					if(!reloc){
						reloc = relocations = (relocation *)calloc(1, sizeof(relocation));
					}
					else{
						reloc->next = (relocation *)calloc(1, sizeof(relocation));
						reloc       = reloc->next;
					}

					reloc->section = sections + section->info;

					if (section->type == SHT_REL) {
						reloc->offset      = rel.r_offset;
						reloc->symbolIndex = rel.r_info >> 8;
						reloc->type        = rel.r_info & 0xff;
						reloc->addend      = 0;
					}
					else{
						reloc->offset      = rela.r_offset;
						reloc->symbolIndex = rela.r_info >> 8;
						reloc->type        = (rela.r_info & 0xff) | RELOC_HAS_ADDEND;
						reloc->addend      = rela.r_addend;
					}
					
					reloc->next = NULL;
				}
				
				break;
			}
		}
	}
	
	/* Resolve names for sections. */
	for(section = sections, i = 0; i < shnum; i++, section++){
		section->name = strtabs[shstrtabIndex] + section->nameIndex;
	}
	
	/* Resolve names and sections for symbols. */
	symbol *symbol;
	
	for(symbol = symbols, i = 0; i < nsyms; i++, symbol++){
		symbol->section = sections + symbol->sectionIndex;
		
		if(symbol->type == STT_SECTION && !symbol->nameIndex){
			symbol->name = strtabs[shstrtabIndex] + symbol->section->nameIndex;
		}
		else{
			symbol->name = strtabs[symstrtabIndex] + symbol->nameIndex;
		}
	}
	
	/* Resolve symbols for relocations. */
	relocation *reloc;
	
	for(reloc = relocations; reloc; reloc = reloc->next){
		reloc->symbol = symbols + reloc->symbolIndex;
	}
	
#ifdef DEBUG
	printf("Sections:\n%3s %-13s %-9s %-9s %-9s %4s\n", "Idx", "Name", "VMA", "Size", "File off", "Algn");
	
	for(section = sections, i = 0; i < shnum; i++, section++){
		printf("%3d %-13s %08lx  %08x  %08x  2**%d\n",
			i, section->name, (paddr_t)section->addr, section->size,
			section->offset, section->alignment
		);
	}
	
	printf("\nSymbols:\n");
	
	for(symbol = symbols, i = 0; i < nsyms; i++, symbol++){
		printf("%08x %c     %c %-6s %08x %s\n",
			symbol->value,
			(symbol->binding      == STB_LOCAL)  ? 'l'     : (symbol->binding == STB_GLOBAL)     ? 'g'     : ' ',
			(symbol->type         == STT_OBJECT) ? 'O'     : (symbol->type == STT_FUNC)          ? 'F'     : ' ',
			(symbol->sectionIndex == SHN_ABS)    ? "*ABS*" : (symbol->sectionIndex == SHN_UNDEF) ? "*UND*" : symbol->section->name,
			symbol->size, symbol->name
		);
	}
	
	printf("\nRelocations:\n%-8s %-18s %s\n", "Offset", "Type", "Value");
	
	for(reloc = relocations; reloc; reloc = reloc->next){
		printf("%08x %-18d %s\n", reloc->offset, reloc->type, reloc->symbol->name);
	}
	
	printf("\n");
#endif /* DEBUG */
	
	/* Perform relocations. */
	for(reloc = relocations; reloc; reloc = reloc->next){
		u8  *addr   = (u8 *)(reloc->section->addr + reloc->offset);
		u16 *addr16 = (u16 *)addr;
		u32 *addr32 = (u32 *)addr;
		
		s32 A = 0;
		u32 At, S, P;
		
		S = (paddr_t)(reloc->symbol->section->addr + reloc->symbol->value);
		P = (paddr_t)(reloc->section->addr + reloc->offset);
		
		/* Extract the addend. */
		switch (reloc->type & 0xff) {
			case R_ARM_NONE: {
				A = 0;
				
				break;
			}
			case R_ARM_PC24: {
				At   = *addr32 & 0xffffff;
				At <<= 2;
				
				if(At & (1 << 25)){
					At |= 0xfc000000;
				}
				
				A = At;
				
				break;
			}
			case R_ARM_ABS32:
			case R_ARM_REL32: {
				if((unsigned long)addr & 3){
					fprintf(stderr, "Unable to handle ABS32 relocation for unaligned address %p. ", addr);
					fprintf(stderr, "Relocation is at (P) %x for (S) %x, section %s, symbol %s.",
						P, S,
						reloc->section->name,
						reloc->symbol->name
					);
					
					return false;
				}
				
				A = *addr32;
				
				break;
			}
			case R_ARM_PC13: {
				A = *addr32 & 0xfff;
				
				if(!(*addr32 & (1 << 23))){
					A = -A;
				}
				
				break;
			}
			case R_ARM_ABS16: {
				A = *addr16;
				
				break;
			}
			case R_ARM_ABS12: {
				A = *addr32 & 0xfff;
				
				break;
			}
			case R_ARM_ABS8: {
				A = *addr;
				
				break;
			}
			case R_ARM_SWI24: {
				A = *addr32 & 0xffffff;
				
				break;
			}
			default: {
				fprintf(stderr, "Unable to handle relocation extraction for type %d (probably bug).", reloc->type & 0xff);
				
				return false;
			}
		}
		
		if(reloc->type & RELOC_HAS_ADDEND){
			A += reloc->addend;
		}
		
		/* Calculate the relocation. */
		u32 value;
		
		switch(reloc->type & 0xff){
			case R_ARM_PC24:
			case R_ARM_REL32:
			case R_ARM_PC13: {
				value = S - P + A;
				
				break;
			}
			case R_ARM_ABS32:
			case R_ARM_ABS16:
			case R_ARM_ABS12:
			case R_ARM_ABS8:
			case R_ARM_SWI24:
			case R_ARM_THM_ABS5:
			case R_ARM_THM_SWI8: {
				value = S + A;
				
				break;
			}
			default: {
				fprintf(stderr, "Unable to handle relocation calculation for type %d (probably bug).", reloc->type & 0xff);
				
				return false;
			}
		}

		/* Insert relocated value. */
		switch(reloc->type & 0xff){
			case R_ARM_NONE: {
				break;
			}
			case R_ARM_PC24: {
				*addr32 &= ~0xffffff;
				*addr32 |= ((value >> 2) & 0xffffff);
				
				break;
			}
			case R_ARM_ABS32:
			case R_ARM_REL32: {
				*addr32 = value;
				
				break;
			}
			case R_ARM_PC13: {
				*addr32 &= ~0xfff;
				
				if(value < 0) {
					*addr32 |= (1 << 23) | (-value & 0xfff);
				}
				else{
					*addr32 &= ~(1 << 23);
					*addr32 |= (value & 0xfff);
				}
				
				break;
			}
			case R_ARM_ABS16: {
				*addr16 = (u16)value;
				
				break;
			}
			case R_ARM_ABS12: {
				*addr32 &= ~0xfff;
				*addr32 |= (value & 0xfff);
				
				break;
			}
			case R_ARM_ABS8: {
				*addr = (u8)value;
				
				break;
			}
			case R_ARM_SWI24: {
				*addr32 &= ~0xffffff;
				*addr32 |= (value & 0xffffff);
				
				break;
			}
			default: {
				fprintf(stderr, "Unable to handle relocation insertion for type %d (probably bug).", reloc->type & 0xff);
				
				return false;
			}
		}

#ifdef DEBUG
		/* Print some status. */
		if(reloc->type & RELOC_HAS_ADDEND){
			A -= reloc->addend;
		}

		printf("Relocating (S) %08x (- (P) %08x) + (A) %08x %s+ (Ad) %08x%s: %08x. Word: %08x.\n",
			S, P, A,
			(reloc->type & RELOC_HAS_ADDEND)? "" : "(",
			(reloc->type & RELOC_HAS_ADDEND)? reloc->addend : 0,
			(reloc->type & RELOC_HAS_ADDEND)? "" : ")",
			value, *addr32
		);
#endif /* DEBUG */
	}
	
	/* Add segment to trace. */
	MemSegment *segment = new MemSegment(0, size, data);
	
	trace->addSegment(segment);
	
	/* Add some symbols to trace. */
	for(symbol = symbols, i = 0; i < nsyms; i++, symbol++){
		if(symbol->name[0] != '.' && symbol->name[0] != '$'){
			trace->create_sym(symbol->name, segment->getBaseAddress() + symbol->value);
		}
	}
	
	/* Free memory. */
	for(i = 0; i < shnum; i++){
		if(strtabs[i]){
			free(strtabs[i]);
		}
	}
	
	free(strtabs);
	free(sections);
	free(data);
	
	for(reloc = relocations; reloc; ){
		relocation *next = reloc->next;
		
		free(reloc);
		
		reloc = next;
	}
	
	/* Success. */
	return true;
}

