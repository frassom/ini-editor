#include "StringUtils.h"

namespace StringUtils {
	void trim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char c) {
			return !std::isspace(c);
		}));

		s.erase(std::find_if(s.rbegin(), s.rend(), [](char c) {
			return !std::isspace(c);
		}).base(), s.end());
	}

	void toLowerCase(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), tolower);
	}
}