#include "IniEditor.h"

void IniEditor::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);

	if (file.is_open())
		loadFromStream(file);
	else
		throw std::runtime_error("Unable to open file");
}

void IniEditor::loadFromStream(std::istream& in) {
	IniMapper mapper;
	mProperties = mapper.from(in);
}

void IniEditor::saveChangesToFile(const std::string& outputFile) {
	saveToFile(outputFile, outputFile);
}

void IniEditor::saveToFile(const std::string& outputFilename, const std::string& sourceFilename) {

	// Prevent writeToFile("","") call
	if (outputFilename.empty())
		throw std::runtime_error("Invalid empty output filename");

	bool isSameFile = outputFilename == sourceFilename;

	// Open a different output file if sourceFilename and outputFilename are the same,
	// cannot read and write to the same file concurrently
	std::ofstream output;
	if (isSameFile)
		output.open("tmp");
	else
		output.open(outputFilename);

	if (!output.is_open())
		throw std::runtime_error("Could not open output file");

	// Write to the ofstream using or not a source file
	if (!sourceFilename.empty()) {

		// Open the input file
		std::ifstream source(sourceFilename);

		if (!source.is_open())
			throw std::runtime_error("Could not open source file");

		saveToStream(output, source);
	}
	else
		saveToStream(output);

	// Rename the temporary file if it was open
	if (isSameFile) {
		if (std::rename("tmp", outputFilename.c_str()))
			throw std::runtime_error("Could not rename temporary file, changes saved to 'tmp'");
	}
}

void IniEditor::saveToStream(std::ostream& output) {
	IniWriter writer;
	writer.write(mProperties, output);
}

void IniEditor::saveToStream(std::ostream& output, std::istream& source) {
	IniWriter writer;
	writer.write(mProperties, output, source);
}

bool IniEditor::get(const std::string& section, const std::string& name, std::string& value) {
	auto it = mProperties.find(IniMapper::makeKey(section, name));
	if (it != mProperties.end()) {
		value = it->second;
		return true;
	}

	return false;
}

void IniEditor::set(const std::string& section, const std::string& name, const std::string& value) {
	if (section.find(']') == std::string::npos && name.find('=') == std::string::npos)
		mProperties[IniMapper::makeKey(section, name)] = value;
	else
		throw std::runtime_error("Invalid section or name syntax");
}

bool IniEditor::remove(const std::string& section, const std::string& name) {
	return mProperties.erase(IniMapper::makeKey(section, name)) != 0;
}

bool IniEditor::has(const std::string& section, const std::string& name) {
	return mProperties.find(IniMapper::makeKey(section, name)) != mProperties.end();
}
