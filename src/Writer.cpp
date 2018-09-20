#include "Writer.h"

namespace ini {

	void Writer::write(const IniMap& properties, std::ostream& output) {
		writePropertiesToStream(properties, output);
	}

	void Writer::write(const IniMap& properties, std::ostream& output, std::istream& source) {
		auto pendingProperties = properties;
		std::string lastSection;

		parse(source, [&](const LineProperties& line) {
			switch (line.type) {
				case LineType::COMMENT: {
					std::string comment = line.raw;
					StringUtils::trim(comment);
					output << comment << std::endl;

					break;
				}
				case LineType::KEY: {
					auto it = pendingProperties.find(Mapper::makeKey(line.section, line.name));
					if (it != pendingProperties.end()) {

						writeProperty(line.name, it->second, output);

						// No more pending, remove
						pendingProperties.erase(it);
					}
					else
						writeProperty(line.name, line.value, output);

					break;
				}
				case LineType::END:
				case LineType::SECTION: {

					std::string prefix = lastSection + ']';
					auto it = pendingProperties.lower_bound(prefix);
					auto startIt = it;

					// Check if end not reached and if it has the same prefix
					if (it != pendingProperties.end() &&
						it->first.compare(0, prefix.size(), prefix) == 0) {

						// Write it in the ostream
						writeProperty(Mapper::getName(it->first), it->second, output);

						it++;
					}

					// Remove all written properties
					pendingProperties.erase(startIt, it);

					lastSection = line.section;

					// Print out the sec
					if (line.type == LineType::SECTION)
						writeSection(line.section, output);

					break;
				}
			}
		});

		// Write left over properties
		writePropertiesToStream(pendingProperties, output);
	}

	void Writer::writePropertiesToStream(const IniMap& properties, std::ostream& output) {
		std::string currentSection, lastSection;
		for (auto& item : properties) {

			currentSection = Mapper::getSection(item.first);
			if (currentSection != lastSection) {
				lastSection = currentSection;
				writeSection(currentSection, output);
			}

			writeProperty(Mapper::getName(item.first), item.second, output);
		}
	}

	void Writer::writeProperty(const std::string& name, const std::string& value, std::ostream& output) {
		output << name << " = " << value << '\n';
	}

	void Writer::writeSection(const std::string& section, std::ostream& output) {
		output << '[' << section << "]\n";
	}

}
