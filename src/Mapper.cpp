#include "Mapper.h"

namespace ini {

	IniMap Mapper::from(std::istream& in) {
		parse(in, *this);
		return std::move(mProperties);
	}

	void Mapper::onNewline(const LineType& type, const std::string& section,
						   const std::string& name, const std::string& value, const std::string&) {
		if (type == LineType::KEY) {
			mProperties.insert(std::make_pair(makeKey(section, name), value));
		}
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
