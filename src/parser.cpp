#include "parser.h"

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

		} catch (ParseException& e) {
			e.setLine(lineNumber);
			throw;
		}

		// Check for stream errors
		if (in.bad())
			throw std::runtime_error("Error while reading from stream");
	}

	LineProperties parseLine(std::string line) {
		LineProperties result;

		StringUtils::trimLeft(line);

		if (line.empty()) {
			result.type = LineType::EMPTY;
		} else {
			switch (line.at(0)) {
				case '#':
				case ';': {
					result.type = LineType::COMMENT;

					result.comment = line.substr(1);
					StringUtils::trim(result.comment);

					break;
				}
				case '[': {
					StringUtils::trimRight(line);

					auto lastChar = line.find(']');
					if (lastChar == line.size() - 1) {
						result.type = LineType::SECTION;

						result.section = line.substr(1, lastChar - 1);
						StringUtils::trim(result.section);
						StringUtils::toLowercase(result.section);

						break;
					} else
						throw ParseException("Invalid section syntax: missing ']' at the end", -1);
				}
				default: {
					auto lastKeyChar = line.find('=');
					if (lastKeyChar < line.size()) {
						result.type = LineType::KEY;

						result.name = line.substr(0, lastKeyChar);
						StringUtils::toLowercase(result.name);
						StringUtils::trimRight(result.name);

						// Search for inline comment
						auto commentStart = line.find(';', lastKeyChar + 1);
						commentStart = std::min(commentStart, line.find('#', lastKeyChar + 1));

						if (commentStart < line.size()) {
							result.value = line.substr(lastKeyChar + 1, commentStart - lastKeyChar - 1);

							result.comment = line.substr(commentStart + 1);
							StringUtils::trim(result.comment);
						} else {
							result.value = line.substr(lastKeyChar + 1);
						}
						StringUtils::trim(result.value);

						break;
					} else
						throw ParseException("Invalid value syntax: missing '='", -1);
				}
			}
		}

		return result;
	}

}	// namespace ini
