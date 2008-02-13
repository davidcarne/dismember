#ifndef _LOADERFACTORY_H_
#define _LOADERFACTORY_H_

#include "trace.h"
#include <map>

class FileLoaderMaker {
public:
	typedef std::map<std::string, FileLoaderMaker *> factories_t;
	typedef factories_t::const_iterator  factories_ci;
	
	FileLoaderMaker(std::string loader_name);
	virtual ~FileLoaderMaker() {};
	/* loads from the loader specified with the loadername
	 *
	 *
	 */
	static bool loadFromFile(std::string loader, FILE * datasrc, Trace * ctx);
	
	/* loads from the loader with the highest priority match
	 * for example, a binaryloader would come up with priority 0,
	 * indicating that its a loader of last resort \
	 */
	static bool autoLoadFromFile(FILE * datasrc, Trace * ctx);

	// HACK - I really don't want to write a proper iterator for this
	// Todo later
	// This should only be used in guis anyhow, so its not a big deal to fixup
	static const factories_t & getMap();
	
protected:
	virtual int matchToFile(FILE * f) const = 0;
	virtual bool loadFromFile(FILE * f, Trace * ctx) = 0;
	
private:
};

#endif

