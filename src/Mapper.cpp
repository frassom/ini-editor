#include "Mapper.h"

namespace ini {

	IniMap Mapper::from(std::istream& in) {
		std::map<std::string, std::string> properties;

		parse(in, [&](const LineProperties& line) {

			if (line.type == LineType::KEY) {
				properties.insert(std::make_pair(makeKey(line.section, line.name), line.value));
			}

		});

		return properties;
	}

	std::string Mapper::makeKey(const std::string& section, const std::string& name) {
		return section + ']' + name;
	}

	std::string Mapper::getName(const std::string& key) {
		return key.substr(key.find(']') + 1, key.size());
	}

	std::string Mapper::getSection(const std::string& key) {
		return key.substr(0, key.find(']'));
	}

}
