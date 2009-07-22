#include <arpa/inet.h>
#include <string.h>
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
	virtual bool loadFromFile(FILE * f, ProjectModel * ctx);
private:
	struct flat_hdr header;
	void reloc(u8 *data, u32 base, FILE *fd);
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

bool FlatLoader::loadFromFile(FILE * loadimg, ProjectModel * ctx)
{
	const u32 loadAddress = 0;
	/* Load the image */
	if (readHeader(loadimg) < 0)
		return false;
	
	u8 *data = (u8 *)malloc(header.bss_end + header.stack_size);
	fseek(loadimg, sizeof(struct flat_hdr), SEEK_SET);
	if ((fread(data, 1, header.data_end, loadimg)) < header.data_end)
		return false;

	int bss_size = header.bss_end - header.data_end;
	memset(data + header.data_end, 0, bss_size);

	reloc(data, loadAddress, loadimg);

	MemSegment *ms;
	ctx->addSegment(ms = new MemSegment(loadAddress, header.data_end,
			data, -1, "TXT"));
	ctx->addSegment(new MemSegment(loadAddress + header.data_end,
			bss_size, data + header.data_end, -1, "BSS"));
	ctx->addSegment(new MemSegment(loadAddress + header.bss_end,
			header.stack_size, data + header.bss_end, -1, "STK"));
	free(data);

	ctx->create_sym("_start", ms->getBaseAddress() + header.entry);
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

void FlatLoader::reloc(u8 *data, u32 base, FILE *fd)
{
	u32 *reloc = (u32 *)malloc(sizeof(u32) * header.reloc_count);
	fseek(fd, header.reloc_start, SEEK_SET);
	fread(reloc, sizeof(u32), header.reloc_count, fd);

	/* we don't relocate zero entries because they are tested against zero,
	 * which causes an endless loop. no known side effects */
	for (u32 i = 0; i < header.reloc_count; ++i) {
		u32 *addr = (u32 *)(data + ntohl(reloc[i]));
		u32 n = ntohl(*addr);
		if (n) *addr = n + base;
	}
	free(reloc);
}
