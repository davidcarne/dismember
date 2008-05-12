#include <arpa/inet.h>
#include "../loaderfactory.h"
#include "memsegment.h"

#define FLTMAGIC "bFLT"

struct flat_hdr {
	char magic[4];
	u32 rev; /* revision */
	u32 entry; /* offset of first instr */
	u32 data_start; /* .data */
	u32 data_end;
	u32 bss_end;
	/* data_end through bss_end forms the bss segment. */
	u32 stack_size; /* in bytes */
	u32 reloc_start; /* records */
	u32 reloc_count;
	u32 flags;
	u32 build_date;
	u32 filler[5];
};

class FlatLoader : public FileLoaderMaker {
public:
	FlatLoader();
	virtual int matchToFile(FILE * f) const;
	virtual bool loadFromFile(FILE * f, Trace * ctx);
private:
	struct flat_hdr header;
	void reloc(long base, FILE *fd);
	int readHeader(FILE *fd);
} registerFlatLoader;

FlatLoader::FlatLoader() : FileLoaderMaker("BFLT")
{
}

int FlatLoader::matchToFile(FILE * f) const
{
	char magic[4];
	fseek(f, 0, SEEK_SET);
	fread(magic, sizeof(char), 4, f);
	if (memcmp(magic, FLTMAGIC, 4) != 0)
		return -1;
	return 54;
}

bool FlatLoader::loadFromFile(FILE * loadimg, Trace * ctx)
{

	/* Load the image */
	if (readHeader(loadimg) < 0)
		return false;
	
	u8 *data = (u8 *)malloc(header.bss_end + header.stack_size);
	fseek(loadimg, sizeof(struct flat_hdr), SEEK_SET);
	if ((fread(data, 1, header.data_end, loadimg)) < header.data_end)
		return false;

	int bss_size = header.bss_end - header.data_end;
	memset(data + header.data_end, 0, bss_size);

	reloc((long)data, loadimg);

#if MULTIPLE_MEM_SEGMENTS_WORK
	ctx->addSegment(new MemSegment(0, header.data_end, data)); /* code */
	ctx->addSegment(new MemSegment(header.data_end, header.bss_end,
			data + header.data_end)); /* bss */
	ctx->addSegment(new MemSegment(header.bss_end, header.stack_size,
			data + header.bss_end)); /* stack */
#else
	ctx->addSegment(new MemSegment(0, header.bss_end + header.stack_size,				data));
#endif
	free(data);

	ctx->create_sym("_start", header.entry);
	//ctx->analyze(header.entry);

	return true;
}

int FlatLoader::readHeader(FILE *fd)
{
	int s = sizeof(struct flat_hdr);
	fseek(fd, 0, SEEK_SET);

	fread(header.magic, sizeof(char), 4, fd);
	if (memcmp(&header.magic, FLTMAGIC, 4) != 0)
		return -1;
	fread(&header.rev, sizeof(u32), 1, fd);
	fread(&header.entry, sizeof(u32), 1, fd);
	fread(&header.data_start, sizeof(u32), 1, fd);
	fread(&header.data_end, sizeof(u32), 1, fd);
	fread(&header.bss_end, sizeof(u32), 1, fd);
	fread(&header.stack_size, sizeof(u32), 1, fd);
	fread(&header.reloc_start, sizeof(u32), 1, fd);
	fread(&header.reloc_count, sizeof(u32), 1, fd);
	fread(&header.flags, sizeof(u32), 1, fd);
	fread(&header.build_date, sizeof(u32), 1, fd);

	header.rev = ntohl(header.rev);
	header.entry = ntohl(header.entry) - s;
	header.data_start = ntohl(header.data_start) - s;
	header.data_end = ntohl(header.data_end) - s;
	header.bss_end = ntohl(header.bss_end) - s;
	header.stack_size = ntohl(header.stack_size);
	header.reloc_start = ntohl(header.reloc_start);
	header.reloc_count = ntohl(header.reloc_count);
	header.flags = ntohl(header.flags);
	header.build_date = ntohl(header.build_date);

	return 0;
}

void FlatLoader::reloc(long base, FILE *fd)
{
	long i, *reloc = (long *)malloc(sizeof(u32) * header.reloc_count);
	fseek(fd, header.reloc_start, SEEK_SET);
	fread(reloc, sizeof(u32), header.reloc_count, fd);

	/* we don't relocate zero entries because they are tested against zero,
	 * which causes an endless loop. no known side effects */
	for (i = 0; i < header.reloc_count; ++i) {
		u32 addr = base + ntohl(reloc[i]);
		u32 n = ntohl(*((u32 *)addr));
		if (n) *((u32 *)addr) = n + base;
	}
	free(reloc);
}
