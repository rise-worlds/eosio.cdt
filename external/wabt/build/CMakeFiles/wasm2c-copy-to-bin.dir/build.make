# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.11.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.11.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/judgefudge/eossdk/wasmsdk/external/wabt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/judgefudge/eossdk/wasmsdk/external/wabt/build

# Utility rule file for wasm2c-copy-to-bin.

# Include the progress variables for this target.
include CMakeFiles/wasm2c-copy-to-bin.dir/progress.make

CMakeFiles/wasm2c-copy-to-bin: wasm2c
	/usr/local/Cellar/cmake/3.11.3/bin/cmake -E make_directory /Users/judgefudge/eossdk/wasmsdk/external/wabt/bin
	/usr/local/Cellar/cmake/3.11.3/bin/cmake -E copy /Users/judgefudge/eossdk/wasmsdk/external/wabt/build/wasm2c /Users/judgefudge/eossdk/wasmsdk/external/wabt/bin

wasm2c-copy-to-bin: CMakeFiles/wasm2c-copy-to-bin
wasm2c-copy-to-bin: CMakeFiles/wasm2c-copy-to-bin.dir/build.make

.PHONY : wasm2c-copy-to-bin

# Rule to build all files generated by this target.
CMakeFiles/wasm2c-copy-to-bin.dir/build: wasm2c-copy-to-bin

.PHONY : CMakeFiles/wasm2c-copy-to-bin.dir/build

CMakeFiles/wasm2c-copy-to-bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wasm2c-copy-to-bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wasm2c-copy-to-bin.dir/clean

CMakeFiles/wasm2c-copy-to-bin.dir/depend:
	cd /Users/judgefudge/eossdk/wasmsdk/external/wabt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/judgefudge/eossdk/wasmsdk/external/wabt /Users/judgefudge/eossdk/wasmsdk/external/wabt /Users/judgefudge/eossdk/wasmsdk/external/wabt/build /Users/judgefudge/eossdk/wasmsdk/external/wabt/build /Users/judgefudge/eossdk/wasmsdk/external/wabt/build/CMakeFiles/wasm2c-copy-to-bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wasm2c-copy-to-bin.dir/depend

