#ifndef __INI_EDITOR_INIPARSER_H
#define __INI_EDITOR_INIPARSER_H

#include <fstream>

#include "StringUtils.h"
#include "ParseException.h"

enum class LineType {
	COMMENT,
	SECTION,
	KEY
};


class NewlineCallback;

class IniParser {
public:

	struct LineProperties {
		LineType type;

		std::string section;
		std::string name;
		std::string value;
	};

	static void parseFile(const std::string& filename, NewlineCallback& callback);

	static void parseStream(std::istream& in, NewlineCallback& callback);

	static LineProperties parseLine(std::string line);
};

class NewlineCallback {
public:
	virtual void onNewline(const LineType& type, const std::string& section,
						   const std::string& name, const std::string& value, const std::string& raw) = 0;
};


#endif //__INI_EDITOR_INIPARSER_H
