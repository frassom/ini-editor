# ini-editor

A simple library to read and write ini files. Written in c++.

## Features and Ini Style

- line comments can start with `#` or `;`, inline comments are not implemented.

- a name cannot contain a `=` character, while a section cannot contain a `]` character.

- name and value can only be divided by `=`.

- duplicate key (*same name same section*) won't give errors but only the first value will be kept.

- whitespace around section, name and value are removed.

- key output is in the form `name = value` (with spaces around `=`).

- when saving with a source stream/file comments are preserved.

- programmatically deleted keys are not saved to output, however if the specified source has a key not present in the editor it will be preserved.

## Usage

Use ini::Editor to load and save ini files.

### Examples

- **Load from file**

```c++
ini::Editor editor;
editor.loadFromFile("path_to_file.ini");
```

- **Access content**

```c++
// Set a new key or overwrite an old one
// if section and name are already present
editor.set("section", "name", "value");

// Get a value
std::string value = "default";
if (editor.get("section", "name", value))
	// Value found. value == "found_value"
else
	// Value not found. value == "default"

// Remove a key
editor.remove("section", "name");

// Check if a key exist
if (editor.has("secition", "name"))
	// Exist
else
	// Do not exist
```

- **Save to file**

```c++
// Save it in a new file
editor.saveToFile("new_file.ini");

// Save it adapting to an existing .ini file
// adding new keys or altering existing ones
// mantaining its structure.
editor.saveToFile("new_file.ini", "existing_source.ini")

// When you want to overwrite the
// existing file just use
editor.saveChangesToFile("file.ini"); // same as editor.saveToFile("file.ini", "file.ini")
```


## Testing

This project uses the GoogleTest framework.
It is required to have the GTest library installed on your system to run tests as it uses the CMake command `find_package()` to find it.


## Note

This is a university project made for an exam.
