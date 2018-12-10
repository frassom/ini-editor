#include "stringUtils.h"

namespace ini { namespace StringUtils {

	void trimLeft(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
	}

	void trimRight(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(),
				s.end());
	}

	void trim(std::string& s) {
		trimLeft(s);
		trimRight(s);
	}

	void toLowercase(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), tolower);
	}

}}	// namespace ini::StringUtils
