#include "Editor.h"

namespace ini {

	void Editor::loadFromFile(const std::string& filename) {
		std::ifstream file(filename);

		if (file.is_open())
			loadFromStream(file);
		else
			throw std::runtime_error("Unable to open file");
	}

	void Editor::loadFromStream(std::istream& in) {

		parse(in, [&](const LineProperties& line) {
			if (line.type == LineType::KEY) {
				mProperties.insert(std::make_pair(makeKey(line.section, line.name), line.value));
			}
		});
	}

	void Editor::saveChangesToFile(const std::string& outputFile) {
		saveToFile(outputFile, outputFile);
	}

	void Editor::saveToFile(const std::string& outputFilename, const std::string& srcFilename) {

		// Prevent writeToFile("","") call
		if (outputFilename.empty())
			throw std::runtime_error("Invalid empty out filename");

		bool isSameFile = outputFilename == srcFilename;

		// Open a different out file if srcFilename and outputFilename are
		// the same, cannot read and write to the same file concurrently
		std::ofstream out;
		if (isSameFile)
			out.open("tmp");
		else
			out.open(outputFilename);

		if (!out.is_open())
			throw std::runtime_error("Could not open out file");

		// Write to the ofstream using or not a source file
		if (!srcFilename.empty()) {

			// Open the input file
			std::ifstream source(srcFilename);

			if (!source.is_open())
				throw std::runtime_error("Could not open source file");

			saveToStream(out, source);
		} else
			saveToStream(out);

		// Rename the temporary file if it was open
		if (isSameFile) {
			if (std::rename("tmp", outputFilename.c_str()))
				throw std::runtime_error("Could not rename temporary file, changes saved to 'tmp'");
		}
	}

	void Editor::saveToStream(std::ostream& out) {
		writePropertiesToStream(mProperties, out);
	}

	void Editor::saveToStream(std::ostream& out, std::istream& source) {
		auto pendingProperties = mProperties;
		std::string lastSection;

		parse(source, [&](const LineProperties& line) {
			switch (line.type) {
				case LineType::COMMENT: {
					out << "# " << line.comment << "\n";

					break;
				}
				case LineType::KEY: {
					auto it = pendingProperties.find(makeKey(line.section, line.name));
					if (it != pendingProperties.end()) {

						writeProperty(line.name, it->second, line.comment, out);

						// No more pending, remove
						pendingProperties.erase(it);
					} else
						writeProperty(line.name, line.value, line.comment, out);

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
						writeProperty(getName(it->first), it->second, {}, out);

						it++;
					}

					// Remove all written properties
					pendingProperties.erase(startIt, it);

					lastSection = line.section;

					// Print out section
					if (line.type == LineType::SECTION)
						writeSection(line.section, out);

					break;
				}
				case LineType::EMPTY: {
					out << "\n";
					break;
				}
			}
		});

		// Write left over properties
		writePropertiesToStream(pendingProperties, out);
	}

	bool Editor::get(const std::string& section, const std::string& name, std::string& value) {
		auto it = mProperties.find(makeKey(section, name));
		if (it != mProperties.end()) {
			value = it->second;
			return true;
		}

		return false;
	}

	void Editor::set(const std::string& section, const std::string& name, const std::string& value) {
		if (section.find(']') == std::string::npos && name.find('=') == std::string::npos)
			mProperties[makeKey(section, name)] = value;
		else
			throw std::runtime_error("Invalid section or name syntax");
	}

	bool Editor::remove(const std::string& section, const std::string& name) {
		return mProperties.erase(makeKey(section, name)) != 0;
	}

	bool Editor::has(const std::string& section, const std::string& name) {
		return mProperties.find(makeKey(section, name)) != mProperties.end();
	}

	std::string Editor::makeKey(const std::string& section, const std::string& name) {
		return section + ']' + name;
	}

	std::string Editor::getName(const std::string& key) {
		return key.substr(key.find(']') + 1, key.size());
	}

	std::string Editor::getSection(const std::string& key) {
		return key.substr(0, key.find(']'));
	}

	void Editor::writePropertiesToStream(const IniMap& properties, std::ostream& out) {
		std::string currentSection, lastSection;
		for (auto& item : properties) {

			currentSection = getSection(item.first);
			if (currentSection != lastSection) {
				lastSection = currentSection;
				writeSection(currentSection, out);
			}

			writeProperty(getName(item.first), item.second, {}, out);
		}
	}

	void Editor::writeProperty(const std::string& name, const std::string& value,
							   const std::string& inlineComment, std::ostream& out) {
		out << name << " = " << value;

		if (!inlineComment.empty())
			out << "\t# " << inlineComment;

		out << '\n';
	}

	void Editor::writeSection(const std::string& section, std::ostream& out) {
		out << '[' << section << "]\n";
	}

}	// namespace ini
