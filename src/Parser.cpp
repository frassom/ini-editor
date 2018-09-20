#include "Parser.h"

namespace ini {

	void parse(std::istream& in, const std::function<void(const LineProperties&)>& callback) {

		int lineNumber = 1;

		try {
			std::string line;

			std::string currentSection;

			while (std::getline(in, line)) {

				LineProperties lineProperties = parseLine(line);

				if (lineProperties.type == LineType::SECTION)
					currentSection = lineProperties.section;

				if (lineProperties.type == LineType::KEY)
					lineProperties.section = currentSection;

				callback(lineProperties);

				lineNumber++;
			}

			// Check if eof has been reached
			if (!in.bad() && in.eof()) {
				LineProperties lineProperties;
				lineProperties.type = LineType::END;

				callback(lineProperties);
			}

		}
		catch (ParseException& e) {
			e.setLine(lineNumber);
			throw;
		}

		// Check for stream errors
		if (in.bad())
			throw std::runtime_error("Error while reading from stream");
	}

	LineProperties parseLine(std::string line) {
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
						StringUtils::toLowercase(result.section);

						result.type = LineType::SECTION;

						break;
					}
					else
						throw ParseException("Invalid section syntax", -1);
				}
				default:
					auto lastKeyChar = line.find('=');
					if (lastKeyChar < line.size()) {

						result.name = line.substr(0, lastKeyChar);
						StringUtils::toLowercase(result.name);
						StringUtils::trim(result.name);

						result.value = line.substr(lastKeyChar + 1, line.size() - 1);
						StringUtils::trim(result.value);

						result.type = LineType::KEY;

						break;
					}
					else
						throw ParseException("Invalid value syntax: missing '='", -1);
			}
		}
		result.raw = std::move(line);

		return result;
	}

}
