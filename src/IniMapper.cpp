#include "IniMapper.h"

IniMap IniMapper::from(std::istream& in) {
	parse(in, *this);
	return std::move(mProperties);
}

void IniMapper::onNewline(const LineType& type, const std::string& section,
						  const std::string& name, const std::string& value, const std::string&) {
	if (type == LineType::KEY) {
		mProperties.insert(std::make_pair(makeKey(section, name), value));
	}
}

std::string IniMapper::makeKey(const std::string& section, const std::string& name) {
	return section + ']' + name;
}

std::string IniMapper::getName(const std::string& key) {
	return key.substr(key.find(']') + 1, key.size());
}

std::string IniMapper::getSection(const std::string& key) {
	return key.substr(0, key.find(']'));
}
