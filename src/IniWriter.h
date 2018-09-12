#ifndef __INI_EDITOR_INIWRITER_H
#define __INI_EDITOR_INIWRITER_H

#include "IniMapper.h"

class IniWriter : public IniParser::NewlineCallback {
public:
	void write(const IniMap& properties, std::ostream& output);

	// properties even if is passed as non-const reference isn't modified
	void write(const IniMap& properties, std::ostream& output, std::istream& source);

protected:
	void onNewline(const LineType& type, const std::string& section, const std::string& name,
				   const std::string& value, const std::string& raw) override;

private:
	void writePropertiesToStream(const IniMap& properties, std::ostream& output);

	void writeProperty(const std::string& name, const std::string& value, std::ostream& output);

	void writeSection(const std::string& section, std::ostream& output);

private:
	std::ostream* mOutputStream = nullptr;

	IniMap mPendingProperties;

	std::string mLastSection;
};


#endif //__INI_EDITOR_INIWRITER_H
