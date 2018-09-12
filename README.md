# ini-editor

A simple library to read and write .ini files, written in c++.

### Usage example
```cpp
#include<IniEditor.h>

int main() {

	IniEditor editor;
	
	// Parse the content of the file inside IniEditor
	editor.loadFromFile("path_to_file.ini");

	// Add new keys or edit old ones
	editor.set("section", "name", "value");
	
	// Get keys value
	std::string value;
	editor.get("section", "name", value);
	
	// ...
	
	// Save it in a new file
	editor.saveToFile("new_file.ini");
	
	// Save it adapting to an existing .ini file
	// adding new keys or altering existing ones
	// mantaining its structure.
	editor.saveToFile("new_file.ini", "existing_source.ini")
	
	// When you want to overwrite the
	// existing file just use
	editor.saveChangesToFile("file.ini");
	
	return 0;
}
```

### INI Style
- Line comments can start with `#` or `;`,
	inline comments are not implemented.

- A name cannot contain a `=` character, while
	a section cannot contain a `]` character.

- Name and value can only be divided by `=`.

- If a duplicate key (*same name same section*) is found
	it will be discarded.

- During parsing whitespaces at the start
	or end of a line are removed, same thing goes for 
	the ones between a name, its value and the `=` character.

- Key output is in the form `name = value` (with spaces around `=`).
