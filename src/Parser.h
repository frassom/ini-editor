#ifndef __INI_EDITOR_PARSER_H
#define __INI_EDITOR_PARSER_H

#include <iostream>
#include <functional>

#include "StringUtils.h"
#include "ParseException.h"

namespace ini {

	enum class LineType {
		COMMENT,
		SECTION,
		KEY,
		END
	};

	struct LineProperties {
		LineType type;

		std::string raw;

		std::string section;
		std::string name;
		std::string value;
	};

	void parse(std::istream& in, const std::function<void(const LineProperties&)>& callback);

	LineProperties parseLine(std::string line);

}


#endif //__INI_EDITOR_PARSER_H
