#ifndef __INI_EDITOR_INIMAPPER_H
#define __INI_EDITOR_INIMAPPER_H

#include <map>

#include "Parser.h"

typedef std::map<std::string, std::string> IniMap;

class IniMapper : public NewlineCallback {
public:
	IniMap from(std::istream& in);

	static std::string makeKey(const std::string& section, const std::string& name);

	static std::string getName(const std::string& key);

	static std::string getSection(const std::string& key);

	void onNewline(const LineType& type, const std::string& section, const std::string& name,
				   const std::string& value,
				   const std::string& raw) override;

private:
	IniMap mProperties;
};


#endif //__INI_EDITOR_INIMAPPER_H
