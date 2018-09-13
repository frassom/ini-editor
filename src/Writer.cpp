#include "Writer.h"

namespace ini {

	void Writer::write(const IniMap& properties, std::ostream& output) {
		writePropertiesToStream(properties, output);
	}

	void Writer::write(const IniMap& properties, std::ostream& output, std::istream& source) {
		mPendingProperties = properties;

		mOutputStream = &output;

		parse(source, *this);

		mOutputStream = nullptr;

		// Write left over properties
		writePropertiesToStream(mPendingProperties, output);
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

	void Writer::onNewline(const LineType& type, const std::string& section, const std::string& name,
						   const std::string& value, const std::string& raw) {

		if (mOutputStream != nullptr) {
			std::ostream& output = *mOutputStream;
			switch (type) {
				case LineType::COMMENT: {
					std::string comment = raw;
					StringUtils::trim(comment);
					output << comment << std::endl;

					break;
				}
				case LineType::KEY: {
					auto it = mPendingProperties.find(Mapper::makeKey(section, name));
					if (it != mPendingProperties.end()) {

						writeProperty(name, it->second, output);

						// No more pending, remove
						mPendingProperties.erase(it);
					}
					else
						writeProperty(name, value, output);

					break;
				}
				case LineType::END:
				case LineType::SECTION: {

					std::string prefix = mLastSection + ']';
					auto it = mPendingProperties.lower_bound(prefix);
					auto startIt = it;

					// Check if end not reached and if it has the same prefix
					if (it != mPendingProperties.end() &&
						it->first.compare(0, prefix.size(), prefix) == 0) {

						// Write it in the ostream
						writeProperty(Mapper::getName(it->first), it->second, output);

						it++;
					}

					// Remove all written properties
					mPendingProperties.erase(startIt, it);

					mLastSection = section;

					// Print out the sec
					if (type == LineType::SECTION)
						writeSection(section, output);

					break;
				}
			}
		}
		else
			throw std::runtime_error("Null output stream, con't write properties");
	}

	void Writer::writeProperty(const std::string& name, const std::string& value, std::ostream& output) {
		output << name << " = " << value << '\n';
	}

	void Writer::writeSection(const std::string& section, std::ostream& output) {
		output << '[' << section << "]\n";
	}

}
