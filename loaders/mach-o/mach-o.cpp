#include <string.h>
#include "../loaderfactory.h"
#include "memsegment.h"

#define	MH_MAGIC	0xfeedface	/* the mach magic number */
#define MH_CIGAM	0xcefaedfe	/* NXSwapInt(MH_MAGIC) */
#define MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe /* NXSwapInt(MH_MAGIC_64) */

/* Constants for the cmd field of all load commands, the type */
#define	LC_SEGMENT	0x1	/* segment of this file to be mapped */
#define	LC_SYMTAB	0x2	/* link-edit stab symbol table info */
#define	LC_SYMSEG	0x3	/* link-edit gdb symbol table info (obsolete) */
#define	LC_THREAD	0x4	/* thread */
#define	LC_UNIXTHREAD	0x5	/* unix thread (includes a stack) */
#define	LC_LOADFVMLIB	0x6	/* load a specified fixed VM shared library */
#define	LC_IDFVMLIB	0x7	/* fixed VM shared library identification */
#define	LC_IDENT	0x8	/* object identification info (obsolete) */
#define LC_FVMFILE	0x9	/* fixed VM file inclusion (internal use) */
#define LC_PREPAGE      0xa     /* prepage command (internal use) */
#define	LC_DYSYMTAB	0xb	/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB	0xc	/* load a dynamically linked shared library */
#define	LC_ID_DYLIB	0xd	/* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER 0xe	/* load a dynamic linker */
#define LC_ID_DYLINKER	0xf	/* dynamic linker identification */
#define	LC_PREBOUND_DYLIB 0x10	/* modules prebound for a dynamically */
/*  linked shared library */
#define	LC_ROUTINES	0x11	/* image routines */
#define	LC_SUB_FRAMEWORK 0x12	/* sub framework */
#define	LC_SUB_UMBRELLA 0x13	/* sub umbrella */
#define	LC_SUB_CLIENT	0x14	/* sub client */
#define	LC_SUB_LIBRARY  0x15	/* sub library */
#define	LC_TWOLEVEL_HINTS 0x16	/* two-level namespace lookup hints */
#define	LC_PREBIND_CKSUM  0x17	/* prebind checksum */

typedef uint32_t	cpu_type_t;
typedef uint32_t	cpu_subtype_t;
typedef uint32_t	cpu_threadtype_t;

struct mach_header
{
	uint32_t magic;
	cpu_type_t cputype;
	cpu_subtype_t cpusubtype;
	uint32_t filetype;
	uint32_t ncmds;
	uint32_t sizeofcmds;
	uint32_t flags;
};
typedef uint32_t vm_prot_t;

struct segment_command
{
	uint32_t cmd;
	uint32_t cmdsize;
	char segname[16];
	uint32_t vmaddr;
	uint32_t vmsize;
	uint32_t fileoff;
	uint32_t filesize;
	vm_prot_t maxprot;
	vm_prot_t initprot;
	uint32_t nsects;
	uint32_t flags;
};

struct symtab_command
{
	uint32_t cmd;
	uint32_t cmdsize;
	uint32_t symoff;
	uint32_t nsyms;
	uint32_t stroff;
	uint32_t strsize;
};

struct nlist
{
	union {
		int32_t n_strx;
	} n_un;
	uint8_t n_type;
	uint8_t n_sect;
	int16_t n_desc;
	uint32_t n_value;
};

void load_symtab(FILE * f, Trace * ctx, long start, long lcstart, uint8_t * data)
{
	struct symtab_command * s = (struct symtab_command *)data;
	printf("Symtab: %x %x %x %x\n", s->symoff, s->nsyms, s->stroff, s->strsize);
	unsigned int i = 0; 
	
	struct nlist * ns = new nlist[s->nsyms];
	char * strtab = new char[s->strsize];
	
	fseek(f, start + s->symoff, SEEK_SET);
	fread(ns, sizeof(struct nlist), s->nsyms, f);
	fseek(f, start + s->stroff, SEEK_SET);
	fread(strtab, 1, s->strsize, f);
	
	
	for (i=0; i< s->nsyms; i++)
	{
		if ((ns[i].n_type & 0xe) == 0xe)
		{
			// Defined symbol
			//printf("%04d: %02x %02x %04x %08x %s\n", i, ns[i].n_type, ns[i].n_sect, ns[i].n_desc, ns[i].n_value, strtab + ns[i].n_un.n_strx);
			address_t at = ctx->locateAddress(ns[i].n_value);
			ctx->create_sym(strtab + ns[i].n_un.n_strx, at);
			
			//ProgramFlowAnalysis::submitAnalysisJob(d, ctx->getCodeDataType(), curr->get_direct_jump_addr());
		}
	}
	
	delete ns;
	delete strtab;
}
void load_seg(FILE * f, Trace * ctx, long start, long lcstart, uint8_t * data)
{
	struct segment_command * s = (struct segment_command *)data;
	char buf[17];
	buf [16] = 0;
	memcpy(buf, &(s->segname), 16);
	
	printf("SEGMENT LOAD: %s  %x %x %x %x\n", buf, s->vmaddr, s->vmsize, s->fileoff, s->filesize);

	fseek(f, start + s->fileoff, SEEK_SET);
	
	uint8_t * segment_mem = new uint8_t[s->filesize];
	fread(segment_mem, s->filesize, 1, f);
	
	MemSegment * msg = new MemSegment(s->vmaddr, s->vmsize, segment_mem, s->filesize);
	delete segment_mem;
	
	ctx->addSegment(msg);
}
class MachOLoader : public FileLoaderMaker {
public:
	MachOLoader();
	virtual int matchToFile(FILE * f) const;
	virtual bool loadFromFile(FILE * f, Trace * ctx);
} registerMachOLoader;

MachOLoader::MachOLoader() : FileLoaderMaker("Mach-O")
{
}

int MachOLoader::matchToFile(FILE * f) const
{
	unsigned long magic = 0;
	fread(&magic, 4, 1, f);
	
	if (magic == MH_MAGIC ||
		magic == MH_CIGAM || 
		magic == MH_MAGIC_64 ||
		magic == MH_CIGAM_64)
		return 100;
	
	return -1;
}

const std::string loadcmdname(uint32_t cmd)
{
	cmd &= ~0x80000000;
	switch (cmd)
	{
	case LC_SEGMENT:
		return "LC_SEGMENT";
	case LC_SYMTAB:
		return "LC_SYMTAB";
	case LC_SYMSEG:
		return "LC_SYMSEG";
	case LC_THREAD:
		return "LC_THREAD";
	case LC_UNIXTHREAD:
		return "LC_UNIXTHREAD";
	case LC_LOADFVMLIB:
		return "LC_LOADFVMLIB";
	case LC_IDFVMLIB:
		return "LC_IDFVMLIB";
	case LC_IDENT:
		return "LC_IDENT";
	case LC_FVMFILE:
		return "LC_FVMFILE";
	case LC_PREPAGE:
		return "LC_PREPAGE";
	case LC_DYSYMTAB:
		return "LC_DYSYMTAB";
	case LC_LOAD_DYLIB:
		return "LC_LOAD_DYLIB";
	case LC_ID_DYLIB:
		return "LC_ID_DYLIB";
	case LC_LOAD_DYLINKER:
		return "LC_LOAD_DYLINKER";
	case LC_ID_DYLINKER:
		return "LC_ID_DYLINKER";
	case LC_PREBOUND_DYLIB:
		return "LC_PREBOUND_DYLIB";
	case LC_ROUTINES:
		return "LC_ROUTINES";
	case LC_SUB_FRAMEWORK:
		return "LC_SUB_FRAMEWORK";
	case LC_SUB_UMBRELLA:
		return "LC_SUB_UMBRELLA";
	case LC_SUB_CLIENT:
		return "LC_SUB_CLIENT";
	case LC_SUB_LIBRARY:
		return "LC_SUB_LIBRARY";
	case LC_TWOLEVEL_HINTS:
		return "LC_TWOLEVEL_HINTS";
	case LC_PREBIND_CKSUM:
		return "LC_PREBIND_CKSUM";
	}
	return "unknown";
}
bool MachOLoader::loadFromFile(FILE * f, Trace * ctx)
{
	
	long start = ftell(f);
	long magic;
	fread(&magic, 4, 1, f);
	
	bool mach64;
	bool needswap;
	
	switch (magic)
	{
		case MH_MAGIC:
			mach64 = false;
			needswap = false;
			break;
		case MH_CIGAM:
			mach64 = false;
			needswap = true;
			break;
			
		case MH_MAGIC_64:
			mach64 = true;
			needswap = false;
			break;
			
		case MH_CIGAM_64:
			mach64 = true;
			needswap = true;
			break;	
		default:
			printf("Not a mach-o file\n");
			return false;
	}
	
	// don't support other stuff yet, as I don't need it
	if (mach64 != false || needswap != false)
		return false;
	
	printf("mach-o load settings: %d %d\n", mach64, needswap);
	fseek(f, start, SEEK_SET);
		
	struct mach_header head;
	fread(&head, sizeof(struct mach_header), 1, f);
	printf("Header: %x %x %x %x %x %x\n", head.cputype, head.cpusubtype, head.filetype, head.ncmds, head.sizeofcmds, head.flags);
	
	unsigned int i = 0;
	for (i=0; i< head.ncmds; i++)
	{
		long lcstart = ftell(f);
		
		uint32_t cmd;
		uint32_t cmdsize;
		fread(&cmd, 4, 1, f);
		fread(&cmdsize, 4, 1, f);
		
		uint8_t * buf = new uint8_t[cmdsize];
		fseek(f, lcstart, SEEK_SET);
		fread(buf, cmdsize, 1, f);
		printf("%d: %x %s %d\n", i, cmd, loadcmdname(cmd).c_str(), cmdsize);
		// Do something here
		
		switch (cmd)
		{
			case LC_SEGMENT:
				load_seg(f, ctx, start, lcstart, buf);
				break;
			case LC_SYMTAB:
				load_symtab(f, ctx, start, lcstart, buf);
				break;
		}
		delete buf;
		
		fseek(f, lcstart+cmdsize, SEEK_SET);
	}
	return true;
}

