#ifndef _LOADERFACTORY_H_
#define _LOADERFACTORY_H_

#include "dsmem_trace.h"
#include <map>

/**
 * Factory + loader for files. This is a runtime pluggable factory. Subfactories register by subclassing + 
 *	instantiating the subclass
 */
class FileLoaderMaker {
public:
	/// list of pluggable factories
	typedef std::map<std::string, FileLoaderMaker *> factories_t;
	/// iterator for list of pluggable factories
	typedef factories_t::const_iterator  factories_ci;
	
	
	virtual ~FileLoaderMaker() {};
	
	/** loads from the loader specified with the loadername
	 * @param loader the loader name
	 * @param datasrc the data source
	 * @param ctx the context to load the data source to
	 */
	static bool loadFromFile(std::string loader, FILE * datasrc, ProjectModel * ctx);
	
	/**
	 * \brief automatically load a given FILE to the trace
	 *
	 *loads from the loader with the highest priority match
	 * for example, a binaryloader would come up with priority 0,
	 * indicating that its a loader of last resort \
	 *
	 * @param datasrc - file data source
	 * @param ctx context to load the datasource to
	 */
	static bool autoLoadFromFile(FILE * datasrc, ProjectModel * ctx);

	/**
	 * get the loader data map. This needs a fixup.
	 * \deprecated
	 * \todo remove me + replace with proper iterator
	 */
	static const factories_t & DEPRECATED(getMap());
	
protected:
	/**
	 * All inheriting factories must implement to tell the factory the quality of match.
	 * @param f FILE may be in any state. user must rewind.
	 * @return 100- perfect, 0 - could load, but last resort. -1, can't load
	 */
	virtual int matchToFile(FILE * f) const = 0;
	/**
	 * All inheriting factories must implement - load the given file to the given trace
	 */
	virtual bool loadFromFile(FILE * f, ProjectModel * ctx) = 0;
	
	/**
	 * Constructor for the pluggable factory used in auto-registration
	 */
	FileLoaderMaker(std::string loader_name);
	

};

#endif

