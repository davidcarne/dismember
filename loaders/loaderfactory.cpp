#include "loaderfactory.h"
#include "types.h"

// We have this NULL, that way its either in initialized DATA, or in BSS.
// would be nice if this could just be a member, but no such luck
// GCC apparently defines an init_order to help fix this, but doesn't appear
// to work on OSX / recent GCC
static FileLoaderMaker::factories_t *m_registry = NULL;

FileLoaderMaker::FileLoaderMaker(std::string loader_name)
{
	if (!m_registry)
		m_registry = new FileLoaderMaker::factories_t();
	m_registry->operator[](loader_name) = this;
}

bool FileLoaderMaker::loadFromFile(std::string loader, FILE * datasrc, ProjectModel * ctx)
{
	factories_ci i = m_registry->find(loader);
	if (i == m_registry->end())
		return false;
	
	return (*i).second->loadFromFile(datasrc, ctx);
}

bool FileLoaderMaker::autoLoadFromFile(FILE * datasrc, ProjectModel * ctx)
{
	factories_ci i = m_registry->begin();
	int cpri = -1;
	FileLoaderMaker * cur = NULL; // suppress spurious compiler warning
	
	for (; i != m_registry->end(); i++)
	{
		int offs = ftell(datasrc);
		
		int pri = (*i).second->matchToFile(datasrc);
		printf("Loader %s returned %d\n", (*i).first.c_str(), pri);
		if (pri > cpri)
		{
			cpri = pri;
			cur  = (*i).second;
		}
		fseek(datasrc, offs, SEEK_SET);
	}
	
	if (cpri == -1)
		return false;
	return cur->loadFromFile(datasrc, ctx);
}

const FileLoaderMaker::factories_t & FileLoaderMaker::getMap()
{
	return *m_registry;
}

