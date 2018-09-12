#include <gtest/gtest.h>

#include "../src/IniWriter.h"

TEST(IniWriterTest, simpleWrite) {
	IniMap properties;
	IniWriter writer;
	std::stringstream output;
	std::string result;

	properties["section1]name1"] = "value1.1";
	properties["section1]name2"] = "value1.2";
	properties["section2]name1"] = "value2.1";

	writer.write(properties, output);
	result = output.str();

	EXPECT_EQ(result, "[section1]\n"
					  "name1 = value1.1\n"
					  "name2 = value1.2\n"
					  "[section2]\n"
					  "name1 = value2.1\n");
}

TEST(IniWriterTest, sourceWriteAddKey) {
	IniMap properties;
	IniWriter writer;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name0=value0\n";
	source << "[end_section]\n";

	properties["section]name"] = "value";

	writer.write(properties, output, source);
	result = output.str();

	EXPECT_EQ(result, "[section]\n"
					  "name0 = value0\n"
					  "name = value\n"
					  "[end_section]\n");
}

TEST(IniWriterTest, sourceWriteAddKeyEnd) {
	IniMap properties;
	IniWriter writer;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name0=value0\n";

	properties["section1]name1"] = "value1";
	properties["section]name"] = "value";

	writer.write(properties, output, source);
	result = output.str();


	EXPECT_EQ(result, "[section]\n"
					  "name0 = value0\n"
					  "name = value\n"
					  "[section1]\n"
					  "name1 = value1\n");
}

TEST(IniWriterTest, sourceWriteAddSection) {
	IniMap properties;
	IniWriter writer;
	std::stringstream source, output;
	std::string result;

	source << "[section0]\n";
	source << "name0 = value0\n";

	properties["section]name"] = "value";

	writer.write(properties, output, source);
	result = output.str();

	EXPECT_EQ(result, "[section0]\n"
					  "name0 = value0\n"
					  "[section]\n"
					  "name = value\n");
}

TEST(IniWriterTest, sourceWriteEditKey) {
	IniMap properties;
	IniWriter writer;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name = value\n";

	properties["section]name"] = "new_value";

	writer.write(properties, output, source);
	result = output.str();

	EXPECT_EQ(result, "[section]\n"
					  "name = new_value\n");
}

TEST(IniWriterTest, sourceWritePreserveComments) {
	IniMap properties;
	IniWriter writer;
	std::stringstream source, output;
	std::string result;

	source << "  # start \t\n";
	source << "[section1]\n";
	source << "name1 = value1\n";
	source << "\t # comment  \n";

	writer.write(properties, output, source);
	result = output.str();

	EXPECT_EQ(result, "# start\n"
					  "[section1]\n"
					  "name1 = value1\n"
					  "# comment\n");
}

TEST(IniWriterTest, dontModifyReferenceMap) {
	IniMap properties;
	IniWriter writer;
	std::stringstream output, source;
	std::string result;

	source << "# start\n";
	source << "[section1]\n";
	source << "name1 = value1\n";
	source << "\t# comment\n";
	source << "[section3]\n";
	source << "name1 = value1\n";

	properties["section1]name1"] = "value1.1";
	properties["section1]name2"] = "value1.2";
	properties["section2]name1"] = "value2.1";

	IniMap propertiesCopy(properties);

	writer.write(properties, output, source);

	EXPECT_EQ(properties, propertiesCopy);
}
