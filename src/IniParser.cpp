#include "IniParser.h"

void IniParser::parseFile(const std::string& filename, NewlineCallback& callback) {
	std::ifstream file(filename);

	if (file.is_open())
		parseStream(file, callback);
	else
		throw std::runtime_error("Could not open file");
}

void IniParser::parseStream(std::istream& in, NewlineCallback& callback) {

	int lineNumber = 0;

	try {
		std::string line;

		std::string currentSection;

		while (std::getline(in, line)) {
			lineNumber++;

			LineProperties lineProp = parseLine(line);

			if (lineProp.type == LineType::SECTION) {
				currentSection = lineProp.section;
			}

			callback.onNewline(lineProp.type, lineProp.type != LineType::COMMENT ? currentSection : "",
							   lineProp.name, lineProp.value, line);

		}
	}
	catch (const std::runtime_error& e) {
		throw ParseException(e.what(), lineNumber);
	}

	// Check for stream errors
	if (in.bad())
		throw std::runtime_error("Error while reading from stream");
}

IniParser::LineProperties IniParser::parseLine(std::string line) {
	LineProperties result;

	StringUtils::trim(line);
	if (line.empty()) {
		result.type = LineType::COMMENT;    // Empty line are treated as comments
	}
	else {
		switch (line.at(0)) {
			case '#':
			case ';':
				result.type = LineType::COMMENT;

				break;
			case '[': {
				// New section found
				auto lastChar = line.find(']');
				if (lastChar == line.size() - 1) {
					result.section = line.substr(1, lastChar - 1);

					// Trim and lowercase the section
					StringUtils::trim(result.section);
					StringUtils::toLowerCase(result.section);

					result.type = LineType::SECTION;

					break;
				}
				else
					throw std::runtime_error("Invalid section syntax");
			}
			default:
				auto lastKeyChar = line.find('=');
				if (lastKeyChar < line.size()) {

					result.name = line.substr(0, lastKeyChar);
					StringUtils::trim(result.name);

					result.value = line.substr(lastKeyChar + 1, line.size() - 1);
					StringUtils::trim(result.value);

					result.type = LineType::KEY;

					break;
				}
				else
					throw std::runtime_error("Invalid value syntax: missing '='");
		}
	}
	return result;
}
