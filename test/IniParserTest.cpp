#include <gtest/gtest.h>

#include "../src/IniParser.h"

class FakeCallback : public IniParser::NewlineCallback {
public:
	FakeCallback() : type(LineType::COMMENT), timeCalled(0) {}

	void onNewline(const LineType& type, const std::string& section, const std::string& name,
				   const std::string& value, const std::string& raw) override {
		if(type != LineType::END) {
			this->type = type;
			this->section = section;
			this->name = name;
			this->value = value;
			this->raw = raw;
			timeCalled++;
		}
	}

	LineType type;
	std::string section;
	std::string name;
	std::string value;
	std::string raw;
	int timeCalled;
};

TEST(IniParserTest, timesCalledCallback) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name1 = value1\n";
	ss << "\t# comment\n";
	ss << "name2 =   \tvalue2\n";
	ss << "\n";
	ss << "[Section2]\n";
	ss << "name1=value1\n";
	IniParser::parse(ss, callback);

	EXPECT_EQ(callback.timeCalled, 7);
}

TEST(IniParserTest, rightCallbackArgsForKey) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "name = value\n";
	IniParser::parse(ss, callback);

	EXPECT_EQ(callback.raw, "name = value");
	EXPECT_EQ(callback.type, LineType::KEY);
	EXPECT_EQ(callback.section, "");
	EXPECT_EQ(callback.name, "name");
	EXPECT_EQ(callback.value, "value");
}

TEST(IniParserTest, rightCallbackArgsForSection) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "[SECTION]\n";
	IniParser::parse(ss, callback);

	EXPECT_EQ(callback.raw, "[SECTION]");
	EXPECT_EQ(callback.type, LineType::SECTION);
	EXPECT_EQ(callback.section, "section");
	EXPECT_EQ(callback.name, "");
	EXPECT_EQ(callback.value, "");
}

TEST(IniParserTest, rightCallbackArgsForComment) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "# comment\n";
	IniParser::parse(ss, callback);

	EXPECT_EQ(callback.raw, "# comment");
	EXPECT_EQ(callback.type, LineType::COMMENT);
	EXPECT_EQ(callback.section, "");
	EXPECT_EQ(callback.name, "");
	EXPECT_EQ(callback.value, "");
}

TEST(IniParserTest, badSectionParseThrow) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name = value\n";
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Sectio]n2 \n";                // ERROR
	ss << "name2=value2\n";

	EXPECT_THROW({
					 try {
						 IniParser::parse(ss, callback);
					 }
					 catch (const ParseException& e) {
						 EXPECT_EQ(e.line(), 5);
						 throw;
					 }
				 }, ParseException);
}

TEST(IniParserTest, badKeyParseThrow) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "name value\n";
	ss << "# comment\n";
	ss << " \t   trim\t =  this \t\t\n";
	ss << "  [Section2] \n";                // ERROR
	ss << "name2=value2\n";

	EXPECT_THROW({
					 try {
						 IniParser::parse(ss, callback);
					 }
					 catch (const ParseException& e) {
						 EXPECT_EQ(e.line(), 2);
						 throw;
					 }
				 }, ParseException);
}

TEST(IniParserTest, lowercaseSectionAndNameOnParse) {
	FakeCallback callback;
	std::stringstream ss;

	ss << "[Section]\n";
	ss << "NAME = value";
	IniParser::parse(ss, callback);

	EXPECT_EQ(callback.name, "name");
	EXPECT_EQ(callback.section, "section");
}

TEST(IniParserTest, trimLineOnParse) {
	IniParser::LineProperties prop;

	prop = IniParser::parseLine("  \t[ \t section  \t]   ");

	EXPECT_EQ(prop.type, LineType::SECTION);
	EXPECT_EQ(prop.section, "section");
}
