#include <gtest/gtest.h>

#include "../src/parser.h"

using namespace ini;

TEST(ParserTest, timesCalledCallback) {
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

TEST(ParserTest, rightCallbackArgsForKey) {
	std::stringstream ss;

	ss << "name = value\t; comment\n";
	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.type, LineType::KEY);
		EXPECT_EQ(line.section, "");
		EXPECT_EQ(line.name, "name");
		EXPECT_EQ(line.value, "value");
		EXPECT_EQ(line.comment, "comment");
	});
}

TEST(ParserTest, rightCallbackArgsForSection) {
	std::stringstream ss;

	ss << "[SECTION]\n";

	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.type, LineType::SECTION);
		EXPECT_EQ(line.section, "section");
		EXPECT_EQ(line.name, "");
		EXPECT_EQ(line.value, "");
		EXPECT_EQ(line.comment, "");
	});
}

TEST(ParserTest, rightCallbackArgsForComment) {
	std::stringstream ss;

	ss << "# comment\n";
	parse(ss, [](const LineProperties& line) {
		if (line.type == LineType::END)
			return;

		EXPECT_EQ(line.type, LineType::COMMENT);
		EXPECT_EQ(line.comment, "comment");
		EXPECT_EQ(line.section, "");
		EXPECT_EQ(line.name, "");
		EXPECT_EQ(line.value, "");
	});
}

TEST(ParserTest, badSectionParseThrow) {
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name = value\n";
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Sectio]n2 \n";	// ERROR
	ss << "name2=value2\n";

	EXPECT_THROW(
		{
			try {
				parse(ss, [](...) {});
			} catch (const ParseException& e) {
				EXPECT_EQ(e.line(), 5);
				throw;
			}
		},
		ParseException);
}

TEST(ParserTest, badKeyParseThrow) {
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name value\n";	// ERROR
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Section2] \n";
	ss << "name2=value2\n";

	EXPECT_THROW(
		{
			try {
				parse(ss, [](...) {});
			} catch (const ParseException& e) {
				EXPECT_EQ(e.line(), 2);
				throw;
			}
		},
		ParseException);
}

TEST(ParserTest, lowercaseSectionAndNameOnParse) {
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

TEST(ParserTest, trimLineOnParse) {
	LineProperties line;

	line = parseLine("  \tname   = value  \t  ; \t  comment\t   ");

	EXPECT_EQ(line.type, LineType::KEY);
	EXPECT_EQ(line.name, "name");
	EXPECT_EQ(line.value, "value");
	EXPECT_EQ(line.comment, "comment");
}
