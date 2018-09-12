#ifndef __INI_EDITOR_INIMAPPER_H
#define __INI_EDITOR_INIMAPPER_H

#include <map>

#include "IniParser.h"

class IniMapper : public IniParser::NewlineCallback {
public:
	std::map<std::string, std::string> fromFile(const std::string& filename);

	std::map<std::string, std::string> fromStream(std::istream& in);

	static std::string makeKey(const std::string& section, const std::string& name);

	static std::string getName(const std::string& key);

	static std::string getSection(const std::string& key);

protected:
	void onNewline(const LineType& type, const std::string& section, const std::string& name, const std::string& value,
				   const std::string& raw) override;

private:
	std::map<std::string, std::string> mProperties;
};


#endif //__INI_EDITOR_INIMAPPER_H
