#ifndef EDITOR__H
#define EDITOR__H

#include <fstream>
#include <map>

#include "parser.h"

namespace ini {

	typedef std::map<std::string, std::string> IniMap;

	class Editor {
	public:
		void loadFromFile(const std::string& filename);

		void loadFromStream(std::istream& in);

		void saveChangesToFile(const std::string& outputFile);

		void saveToFile(const std::string& outputFilename, const std::string& sourceFilename = "");

		void saveToStream(std::ostream& out);

		void saveToStream(std::ostream& out, std::istream& source);

		bool get(const std::string& section, const std::string& name, std::string& value);

		void set(const std::string& section, const std::string& name, const std::string& value);

		bool remove(const std::string& section, const std::string& name);

		bool has(const std::string& section, const std::string& name);

		static std::string makeKey(const std::string& section, const std::string& name);

		static std::string getName(const std::string& key);

		static std::string getSection(const std::string& key);

	private:
		void writePropertiesToStream(const IniMap& properties, std::ostream& out);

		void writeProperty(const std::string& name, const std::string& value,
						   const std::string& inlineComment, std::ostream& out);

		void writeSection(const std::string& section, std::ostream& out);

	private:
		IniMap mProperties;
	};

}	// namespace ini

#endif
