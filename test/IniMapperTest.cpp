#include <gtest/gtest.h>

#include "../src/IniMapper.h"

TEST(IniMapperTest, mapSize) {
	IniMapper mapper;
	std::stringstream ss;
	std::map<std::string, std::string> map;

	ss << "[Section]\n";
	ss << "name1 = value1\n";
	ss << "\t# comment\n";
	ss << "name2 =   \tvalue2\n";
	ss << "\n";
	ss << "[Section2]\n";
	ss << "name1=value1\n";

	EXPECT_NO_THROW(map = mapper.fromStream(ss));
	EXPECT_EQ(map.size(), 3);
}

TEST(IniMapperTest, propertyValue) {
	IniMapper mapper;
	std::stringstream ss;
	std::map<std::string, std::string> map;

	ss << "name0 = value0\n";
	ss << " # comment\n";
	ss << "name1 = value1\n";
	ss << "\n";
	ss << "[Section]\n";
	ss << "	\t name1  =\tvalue1  \n";

	EXPECT_NO_THROW(map = mapper.fromStream(ss));
	auto it = map.find("name1=section");

	EXPECT_TRUE(it != map.end());
	EXPECT_EQ(it->second, "value1");
}

TEST(IniMapperTest, makeKey) {
	EXPECT_EQ(IniMapper::makeKey("section", "name"), "name=section");
}
