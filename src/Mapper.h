#ifndef __INI_EDITOR_INIMAPPER_H
#define __INI_EDITOR_INIMAPPER_H

#include <map>

#include "Parser.h"

namespace ini {

	typedef std::map<std::string, std::string> IniMap;

	class Mapper {
	public:
		IniMap from(std::istream& in);

		static std::string makeKey(const std::string& section, const std::string& name);

		static std::string getName(const std::string& key);

		static std::string getSection(const std::string& key);
	};

}


#endif //__INI_EDITOR_INIMAPPER_H
