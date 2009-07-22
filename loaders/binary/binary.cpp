#include "../loaderfactory.h"
#include "memsegment.h"
#include "guidialog.h"

class BinaryLoader : public FileLoaderMaker {
public:
	BinaryLoader();
	virtual int matchToFile(FILE * f) const;
	virtual bool loadFromFile(FILE * f, ProjectModel * ctx);
} registerBinaryLoader;

BinaryLoader::BinaryLoader() : FileLoaderMaker("Binary")
{
	
}

int BinaryLoader::matchToFile(FILE * f) const
{
	return 0;
}

bool BinaryLoader::loadFromFile(FILE * loadimg, ProjectModel * ctx)
{
	uint64_t addr = 0;
	guiDialog *base = createGuiDialog("Input base address");
	base->addTextInput("addr", "Base address:", "0x00000000");
	base->addSubmit("OK");
	base->showModal();

	AbstractData &var = base->getValue("addr");
	std::string str = boost::get<std::string>(var);
	if (str.length() > 0)
		addr = strtol(str.c_str(), 0, 0);
	
	
	u32 len;
	
	/* Load the image */
	fseek(loadimg, 0L, SEEK_END);
	len = ftell(loadimg);
	rewind(loadimg);
	
	u8 *data = (u8*)malloc(len);
	
	if ((fread(data, 1, len, loadimg)) < len) {
		if (ferror(loadimg))
			return false;
	}
	
	MemSegment * msg = new MemSegment(addr, len, data);
	free(data);
	
	ctx->addSegment(msg);
	
	return true;
}

