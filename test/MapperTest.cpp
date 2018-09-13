#include <gtest/gtest.h>

#include "../src/Mapper.h"

using namespace ini;

TEST(IniMapperTest, mapSize) {
	Mapper mapper;
	std::stringstream ss;
	IniMap map;

	ss << "[Section]\n";
	ss << "name1 = value1\n";
	ss << "\t# comment\n";
	ss << "name2 =   \tvalue2\n";
	ss << "\n";
	ss << "[Section2]\n";
	ss << "name1=value1\n";

	EXPECT_NO_THROW(map = mapper.from(ss));
	EXPECT_EQ(map.size(), 3);
}

TEST(IniMapperTest, propertyValue) {
	Mapper mapper;
	std::stringstream ss;
	IniMap map;

	ss << "name0 = value0\n";
	ss << " # comment\n";
	ss << "name1 = value1\n";
	ss << "\n";
	ss << "[Section]\n";
	ss << "	\t name1  =\tvalue1  \n";

	EXPECT_NO_THROW(map = mapper.from(ss));
	auto it = map.find("section]name1");

	EXPECT_TRUE(it != map.end());
	EXPECT_EQ(it->second, "value1");
}

TEST(IniMapperTest, makeKey) {
	EXPECT_EQ(Mapper::makeKey("section", "name"), "section]name");
}

TEST(IniMapperTest, getNameFromKey) {
	EXPECT_EQ(Mapper::getName("section]name"), "name");
}

TEST(IniMapperTest, getSectionFromKey) {
	EXPECT_EQ(Mapper::getSection("section]name"), "section");
}