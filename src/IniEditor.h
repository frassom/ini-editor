#ifndef __INI_EDITOR_INIEDITOR_H
#define __INI_EDITOR_INIEDITOR_H

#include <fstream>

#include "Writer.h"

namespace ini {

	class IniEditor {
	public:
		void loadFromFile(const std::string& filename);

		void loadFromStream(std::istream& in);

		void saveChangesToFile(const std::string& outputFile);

		void saveToFile(const std::string& outputFilename, const std::string& sourceFilename = "");

		void saveToStream(std::ostream& output);

		void saveToStream(std::ostream& output, std::istream& source);

		bool get(const std::string& section, const std::string& name, std::string& value);

		void set(const std::string& section, const std::string& name, const std::string& value);

		bool remove(const std::string& section, const std::string& name);

		bool has(const std::string& section, const std::string& name);

	private:
		IniMap mProperties;
	};

}


#endif //__INI_EDITOR_INIEDITOR_H
