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

# Include any dependencies generated for this target.
include CMakeFiles/wasm-validate.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wasm-validate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wasm-validate.dir/flags.make

CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o: CMakeFiles/wasm-validate.dir/flags.make
CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o: ../src/tools/wasm-validate.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/judgefudge/eossdk/wasmsdk/external/wabt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o -c /Users/judgefudge/eossdk/wasmsdk/external/wabt/src/tools/wasm-validate.cc

CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/judgefudge/eossdk/wasmsdk/external/wabt/src/tools/wasm-validate.cc > CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.i

CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/judgefudge/eossdk/wasmsdk/external/wabt/src/tools/wasm-validate.cc -o CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.s

# Object files for target wasm-validate
wasm__validate_OBJECTS = \
"CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o"

# External object files for target wasm-validate
wasm__validate_EXTERNAL_OBJECTS =

wasm-validate: CMakeFiles/wasm-validate.dir/src/tools/wasm-validate.cc.o
wasm-validate: CMakeFiles/wasm-validate.dir/build.make
wasm-validate: libwabt.a
wasm-validate: CMakeFiles/wasm-validate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/judgefudge/eossdk/wasmsdk/external/wabt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable wasm-validate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wasm-validate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wasm-validate.dir/build: wasm-validate

.PHONY : CMakeFiles/wasm-validate.dir/build

CMakeFiles/wasm-validate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wasm-validate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wasm-validate.dir/clean

CMakeFiles/wasm-validate.dir/depend:
	cd /Users/judgefudge/eossdk/wasmsdk/external/wabt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/judgefudge/eossdk/wasmsdk/external/wabt /Users/judgefudge/eossdk/wasmsdk/external/wabt /Users/judgefudge/eossdk/wasmsdk/external/wabt/build /Users/judgefudge/eossdk/wasmsdk/external/wabt/build /Users/judgefudge/eossdk/wasmsdk/external/wabt/build/CMakeFiles/wasm-validate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wasm-validate.dir/depend

