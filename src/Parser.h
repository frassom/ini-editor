#ifndef __INI_EDITOR_PARSER_H
#define __INI_EDITOR_PARSER_H

#include "iostream"

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

		std::string section;
		std::string name;
		std::string value;
	};

	class NewlineCallback {
	public:
		virtual void onNewline(const LineType& type, const std::string& section,
							   const std::string& name, const std::string& value, const std::string& raw) = 0;
	};

	void parse(std::istream& in, NewlineCallback& callback);

	LineProperties parseLine(std::string line);

}


#endif //__INI_EDITOR_PARSER_H
