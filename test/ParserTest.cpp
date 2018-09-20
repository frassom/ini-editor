#include <gtest/gtest.h>

#include "../src/Parser.h"

using namespace ini;

TEST(IniParserTest, timesCalledCallback) {
	std::stringstream ss;
	int timesCalled = 0;

	ss << "[Section]\n";
	ss << "name1 = value1\n";
	ss << "\t# comment\n";
	ss << "name2 =   \tvalue2\n";
	ss << "\n";
	ss << "[Section2]\n";
	ss << "name1=value1\n";
	parse(ss, [&](const LineProperties& line) {
		if (line.type != LineType::END)
			timesCalled++;
	});

	EXPECT_EQ(timesCalled, 7);
}

TEST(IniParserTest, rightCallbackArgsForKey) {
	std::stringstream ss;

	ss << "name = value\n";
	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.raw, "name = value");
		EXPECT_EQ(line.type, LineType::KEY);
		EXPECT_EQ(line.section, "");
		EXPECT_EQ(line.name, "name");
		EXPECT_EQ(line.value, "value");
	});

}

TEST(IniParserTest, rightCallbackArgsForSection) {
	std::stringstream ss;

	ss << "[SECTION]\n";

	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.raw, "[SECTION]");
		EXPECT_EQ(line.type, LineType::SECTION);
		EXPECT_EQ(line.section, "section");
		EXPECT_EQ(line.name, "");
		EXPECT_EQ(line.value, "");
	});
}

TEST(IniParserTest, rightCallbackArgsForComment) {
	std::stringstream ss;

	ss << "# comment\n";
	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.raw, "# comment");
		EXPECT_EQ(line.type, LineType::COMMENT);
		EXPECT_EQ(line.section, "");
		EXPECT_EQ(line.name, "");
		EXPECT_EQ(line.value, "");
	});
}

TEST(IniParserTest, badSectionParseThrow) {
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name = value\n";
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Sectio]n2 \n";                // ERROR
	ss << "name2=value2\n";

	EXPECT_THROW({
					 try {
						 parse(ss, [](...) {});
					 }
					 catch (const ParseException& e) {
						 EXPECT_EQ(e.line(), 5);
						 throw;
					 }
				 }, ParseException);
}

TEST(IniParserTest, badKeyParseThrow) {
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name value\n";
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Section2] \n";                // ERROR
	ss << "name2=value2\n";

	EXPECT_THROW({
					 try {
						 parse(ss, [](...) {});
					 }
					 catch (const ParseException& e) {
						 EXPECT_EQ(e.line(), 2);
						 throw;
					 }
				 }, ParseException);
}

TEST(IniParserTest, lowercaseSectionAndNameOnParse) {
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "NAME = value";

	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::KEY) {
			EXPECT_EQ(line.name, "name");
			EXPECT_EQ(line.section, "section");
		}
	});
}

TEST(IniParserTest, trimLineOnParse) {
	LineProperties prop;

	prop = parseLine("  \t[ \t section  \t]   ");

	EXPECT_EQ(prop.type, LineType::SECTION);
	EXPECT_EQ(prop.section, "section");
	EXPECT_EQ(prop.raw, "[ \t section  \t]");
}
