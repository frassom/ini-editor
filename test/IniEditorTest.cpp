#include <gtest/gtest.h>

#include "../src/IniEditor.h"

using namespace ini;

TEST(IniEditorTest, shouldHasKey) {
	IniEditor editor;
	std::stringstream ss;

	ss << "[section1]\n";
	ss << "key3 = some_value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.has("section1", "key3"));
}

TEST(IniEditorTest, getValueFromName) {
	IniEditor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.get("section", "name", value));
	EXPECT_EQ(value, "value");
}

TEST(IniEditorTest, editExistingValue) {
	IniEditor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	editor.set("section", "name", "new_value");
	editor.get("section", "name", value);

	EXPECT_EQ(value, "new_value");
}

TEST(IniEditorTest, addNewValue) {
	IniEditor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	editor.set("section", "new_name", "new_value");

	EXPECT_TRUE(editor.get("section", "new_name", value));
	EXPECT_EQ(value, "new_value");
}

TEST(IniEditorTest, removeExistingKey) {
	IniEditor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.has("section", "name"));

	EXPECT_TRUE(editor.remove("section", "name"));

	EXPECT_FALSE(editor.has("section", "name"));
}

TEST(IniEditorTest, tryRemoveNonExistingKey) {
	IniEditor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_FALSE(editor.remove("wrong_section", "wrong_name"));
}

TEST(IniEditorTest, tryInsertingInvalidName) {
	IniEditor editor;

	EXPECT_THROW(editor.set("section", "nam=e", "value"), std::runtime_error);
}

TEST(IniEditorTest, tryInsertingInvalidSection) {
	IniEditor editor;

	EXPECT_THROW(editor.set("sect]ion", "name", "value"), std::runtime_error);
}
