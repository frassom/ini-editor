#ifndef PARSER__H
#define PARSER__H

#include <functional>
#include <iostream>

#include "ParseException.h"
#include "stringUtils.h"

namespace ini {

	enum class LineType { EMPTY, COMMENT, SECTION, KEY, END };

	struct LineProperties {
		LineType type;

		std::string section;
		std::string name;
		std::string value;
		std::string comment;
	};

	void parse(std::istream& in, const std::function<void(const LineProperties&)>& callback);

	LineProperties parseLine(std::string line);

}	// namespace ini

#endif
