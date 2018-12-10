#ifndef STRINGUTILS__H
#define STRINGUTILS__H

#include <algorithm>
#include <string>

namespace ini { namespace StringUtils {

	void trimLeft(std::string&);

	void trimRight(std::string&);

	void trim(std::string&);

	void toLowercase(std::string&);

}}	// namespace ini::StringUtils

#endif
