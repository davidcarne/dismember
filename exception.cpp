#include <string.h>
#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#ifdef __GNUC__
#include <cxxabi.h>
#endif
#include "exception.h"

Exception::Exception(const char *m)
{
	message = strdup(m);
	int nptrs = backtrace(buffer, EXCEPTION_BUFSIZ);
	char const ** strings;
	strings = (const char **)backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		return;
	}

	for (int i = 1; i < nptrs; ++i) {
#ifdef __GNUC__
		const char *str = strings[i];
		const char *s = 0, *e = 0;
		for (const char *j = strings[i]; *j; ++j) {
			if (*j == '(') s = j + 1;
			if (*j == '+') e = j;
		}
		if (s && e) {
			int status;
			size_t size = 512; 
			char *dem = (char *)malloc(size);
			char *full = (char *)malloc(size);
			strncpy(full, s, e - s);
			full[e - s] = 0;
			char *ret = abi::__cxa_demangle(full, dem, &size, &status);
			strncpy(full, str, s - str);
			if (ret) {
				int pos = (s - str);
				snprintf(full + pos, size - pos, "%s%s", ret,e);
				free(ret);
			}
			else {
				int pos = (s - str);
				strncpy(full + pos, s, e - s);
				pos += e - s;
				snprintf(full + pos, size - pos, "()%s", e);
				free(dem);
			}
			stackTrace.push_back(std::string(full));
			free(full);
		}
		else
#endif
			stackTrace.push_back(std::string(strings[i]));
	}
	free(strings);
}

const char *Exception::getMessage() const
{
	return message;
}

void Exception::printStackTrace() const
{
	std::list<std::string>::const_iterator it = stackTrace.begin();
	for (; it != stackTrace.end(); ++it)
		fprintf(stderr, "%s\n", (*it).c_str()); 
}

const std::list<std::string> &Exception::getStackTrace() const
{
	return stackTrace;
}

Exception::~Exception()
{
	free(message);
}
