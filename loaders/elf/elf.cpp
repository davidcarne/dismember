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
	unsigned int       addend;
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
	unsigned int    sectionIndex;
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

ElfLoader::ElfLoader() : FileLoaderMaker("ELF") {
}

int ElfLoader::matchToFile(FILE * file) const {
	Elf32_Ehdr eh;
	
	/* Read ELF header. */
	fseek(file, 0, SEEK_SET);
	if(fread(&eh, sizeof(Elf32_Ehdr), 1, file) != 1){
		return -1;
	}
	
	/* Check if header is supported. */
	if(IS_ELF(eh) &&
	   eh.e_ident[EI_CLASS] == ELFCLASS32  &&
	   eh.e_ident[EI_DATA]  == ELFDATA2LSB &&
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
	fseek(file, 0, SEEK_SET);
	if(fread(&eh, sizeof(Elf32_Ehdr), 1, file) != 1){
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
	else if(eh.e_ident[EI_DATA] != ELFDATA2LSB){
		fprintf(stderr, "Not an LSB ELF file (%d).\n", eh.e_ident[EI_DATA]);
		
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
		if(fread(&sh, sizeof(Elf32_Shdr), 1, file) != 1){
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
					if(fread(&st, 1, section->entsize, file) != section->entsize){
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
						if(fread(&rel, sizeof(Elf32_Rel), 1, file) != 1){
							fprintf(stderr, "Failed to read relocation entry.\n");
							
							return false;
						}
					}
					else{
						if(fread(&rela, sizeof(Elf32_Rela), 1, file) != 1){
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

