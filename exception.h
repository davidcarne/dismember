#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <list>
#include <string>

#define EXCEPTION_BUFSIZ 100

class Exception
{
 public:
	Exception(const char *message);
	virtual ~Exception();
	const char *getMessage() const;
	void printStackTrace() const;
	const std::list<std::string> &getStackTrace() const;

 private:
	char *message;
	std::list<std::string> stackTrace;
	void *buffer[EXCEPTION_BUFSIZ];
};

#endif
