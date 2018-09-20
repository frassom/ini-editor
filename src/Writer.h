#ifndef __INI_EDITOR_INIWRITER_H
#define __INI_EDITOR_INIWRITER_H

#include "Mapper.h"

namespace ini {

	class Writer {
	public:
		void write(const IniMap& properties, std::ostream& output);

		void write(const IniMap& properties, std::ostream& output, std::istream& source);

	private:
		void writePropertiesToStream(const IniMap& properties, std::ostream& output);

		void writeProperty(const std::string& name, const std::string& value, std::ostream& output);

		void writeSection(const std::string& section, std::ostream& output);
	};

}


#endif //__INI_EDITOR_INIWRITER_H
