#ifndef _ABSTRACT_DATA_H_
#define _ABSTRACT_DATA_H_

#include <string>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include "types.h"

typedef boost::variant<int, bool, std::string, address_t> AbstractData;

#endif
