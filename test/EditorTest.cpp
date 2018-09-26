#include <gtest/gtest.h>

#include "../src/Editor.h"

using namespace ini;

TEST(EditorTest, loadCorrectNoThrow) {
	std::stringstream ss;
	Editor editor;

	ss << "name0 = value0\n";
	ss << " # comment\n";
	ss << "name1 = value1\n";
	ss << "\n";
	ss << "[Section]\n";
	ss << "	\t name1  =\tvalue1  \n";
	ss << "name2  =\tvalue1  \n";
	ss << "[Sectio2]\n";
	ss << "name1=value1\n";

	EXPECT_NO_THROW(editor.loadFromStream(ss));
}

TEST(EditorTest, shouldHasKey) {
	Editor editor;
	std::stringstream ss;

	ss << "[section1]\n";
	ss << "key3 = some_value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.has("section1", "key3"));
}

TEST(EditorTest, getValueFromName) {
	Editor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.get("section", "name", value));
	EXPECT_EQ(value, "value");
}

TEST(EditorTest, editExistingValue) {
	Editor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	editor.set("section", "name", "new_value");
	editor.get("section", "name", value);

	EXPECT_EQ(value, "new_value");
}

TEST(EditorTest, addNewValue) {
	Editor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	editor.set("section", "new_name", "new_value");

	EXPECT_TRUE(editor.get("section", "new_name", value));
	EXPECT_EQ(value, "new_value");
}

TEST(EditorTest, removeExistingKey) {
	Editor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_TRUE(editor.has("section", "name"));
	EXPECT_TRUE(editor.remove("section", "name"));
	EXPECT_FALSE(editor.has("section", "name"));
}

TEST(EditorTest, tryRemoveNonExistingKey) {
	Editor editor;
	std::stringstream ss;
	std::string value;

	ss << "[section]\n";
	ss << "name = value\n";
	editor.loadFromStream(ss);

	EXPECT_FALSE(editor.remove("wrong_section", "wrong_name"));
}

TEST(EditorTest, tryInsertingInvalidName) {
	Editor editor;

	EXPECT_THROW(editor.set("section", "nam=e", "value"), std::runtime_error);
}

TEST(EditorTest, tryInsertingInvalidSection) {
	Editor editor;

	EXPECT_THROW(editor.set("sect]ion", "name", "value"), std::runtime_error);
}

TEST(EditorTest, makeKey) {
	EXPECT_EQ(Editor::makeKey("section", "name"), "section]name");
}

TEST(EditorTest, getNameFromKey) {
	EXPECT_EQ(Editor::getName("section]name"), "name");
}

TEST(EditorTest, getSectionFromKey) {
	EXPECT_EQ(Editor::getSection("section]name"), "section");
}

TEST(EditorTest, simpleSave) {
	Editor editor;
	std::stringstream output;
	std::string result;

	editor.set("section1", "name1", "value1.1");
	editor.set("section1", "name2", "value1.2");
	editor.set("section2", "name1", "value2.1");

	editor.saveToStream(output);
	result = output.str();

	EXPECT_EQ(result, "[section1]\n"
					  "name1 = value1.1\n"
					  "name2 = value1.2\n"
					  "[section2]\n"
					  "name1 = value2.1\n");
}

TEST(EditorTest, sourceSaveAddKey) {
	Editor editor;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name0=value0\n";
	source << "[end_section]\n";

	editor.set("section", "name", "value");

	editor.saveToStream(output, source);
	result = output.str();

	EXPECT_EQ(result, "[section]\n"
					  "name0 = value0\n"
					  "name = value\n"
					  "[end_section]\n");
}

TEST(EditorTest, sourceSaveAddKeyEnd) {
	Editor editor;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name0=value0\n";

	editor.set("section1", "name1", "value1");
	editor.set("section", "name", "value");

	editor.saveToStream(output, source);
	result = output.str();

	EXPECT_EQ(result, "[section]\n"
					  "name0 = value0\n"
					  "name = value\n"
					  "[section1]\n"
					  "name1 = value1\n");
}

TEST(EditorTest, sourceSaveAddSection) {
	Editor editor;
	std::stringstream source, output;
	std::string result;

	source << "[section0]\n";
	source << "name0 = value0\n";

	editor.set("section", "name", "value");

	editor.saveToStream(output, source);
	result = output.str();

	EXPECT_EQ(result, "[section0]\n"
					  "name0 = value0\n"
					  "[section]\n"
					  "name = value\n");
}

TEST(EditorTest, sourceSaveEditKey) {
	Editor editor;
	std::stringstream source, output;
	std::string result;

	source << "[section]\n";
	source << "name = value\n";

	editor.set("section", "name", "new_value");

	editor.saveToStream(output, source);
	result = output.str();

	EXPECT_EQ(result, "[section]\n"
					  "name = new_value\n");
}

TEST(EditorTest, sourceSavePreserve) {
	Editor editor;
	std::stringstream source, output;
	std::string result;

	source << "  # start \t\n";
	source << "[section1]\n";
	source << "name1 = value1\n";
	source << "\t # comment  \n";

	editor.saveToStream(output, source);
	result = output.str();

	EXPECT_EQ(result, "# start\n"
					  "[section1]\n"
					  "name1 = value1\n"
					  "# comment\n");
}
