# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xina/workspace/treasure/mycode/code/data_structure/DFS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DFS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DFS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DFS.dir/flags.make

CMakeFiles/DFS.dir/main.cpp.o: CMakeFiles/DFS.dir/flags.make
CMakeFiles/DFS.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DFS.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DFS.dir/main.cpp.o -c /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/main.cpp

CMakeFiles/DFS.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DFS.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/main.cpp > CMakeFiles/DFS.dir/main.cpp.i

CMakeFiles/DFS.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DFS.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/main.cpp -o CMakeFiles/DFS.dir/main.cpp.s

CMakeFiles/DFS.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/DFS.dir/main.cpp.o.requires

CMakeFiles/DFS.dir/main.cpp.o.provides: CMakeFiles/DFS.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/DFS.dir/build.make CMakeFiles/DFS.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/DFS.dir/main.cpp.o.provides

CMakeFiles/DFS.dir/main.cpp.o.provides.build: CMakeFiles/DFS.dir/main.cpp.o


# Object files for target DFS
DFS_OBJECTS = \
"CMakeFiles/DFS.dir/main.cpp.o"

# External object files for target DFS
DFS_EXTERNAL_OBJECTS =

DFS: CMakeFiles/DFS.dir/main.cpp.o
DFS: CMakeFiles/DFS.dir/build.make
DFS: CMakeFiles/DFS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DFS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DFS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DFS.dir/build: DFS

.PHONY : CMakeFiles/DFS.dir/build

CMakeFiles/DFS.dir/requires: CMakeFiles/DFS.dir/main.cpp.o.requires

.PHONY : CMakeFiles/DFS.dir/requires

CMakeFiles/DFS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DFS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DFS.dir/clean

CMakeFiles/DFS.dir/depend:
	cd /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xina/workspace/treasure/mycode/code/data_structure/DFS /Users/xina/workspace/treasure/mycode/code/data_structure/DFS /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug /Users/xina/workspace/treasure/mycode/code/data_structure/DFS/cmake-build-debug/CMakeFiles/DFS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DFS.dir/depend

