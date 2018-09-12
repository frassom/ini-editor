#ifndef __INI_EDITOR_INIPARSER_H
#define __INI_EDITOR_INIPARSER_H

#include "iostream"

#include "StringUtils.h"
#include "ParseException.h"

enum class LineType {
	COMMENT,
	SECTION,
	KEY
};

class IniParser {
public:

	class NewlineCallback;

	struct LineProperties {
		LineType type;

		std::string section;
		std::string name;
		std::string value;
	};

	static void parse(std::istream& in, NewlineCallback& callback);

	static LineProperties parseLine(std::string line);
};

class IniParser::NewlineCallback {
protected:
	virtual void onNewline(const LineType& type, const std::string& section,
						   const std::string& name, const std::string& value, const std::string& raw) = 0;

	friend IniParser;
};


#endif //__INI_EDITOR_INIPARSER_H
