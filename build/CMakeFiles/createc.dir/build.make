# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build

# Include any dependencies generated for this target.
include CMakeFiles/createc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/createc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/createc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/createc.dir/flags.make

CMakeFiles/createc.dir/src/createc.cpp.o: CMakeFiles/createc.dir/flags.make
CMakeFiles/createc.dir/src/createc.cpp.o: /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/src/createc.cpp
CMakeFiles/createc.dir/src/createc.cpp.o: CMakeFiles/createc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/createc.dir/src/createc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/createc.dir/src/createc.cpp.o -MF CMakeFiles/createc.dir/src/createc.cpp.o.d -o CMakeFiles/createc.dir/src/createc.cpp.o -c /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/src/createc.cpp

CMakeFiles/createc.dir/src/createc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/createc.dir/src/createc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/src/createc.cpp > CMakeFiles/createc.dir/src/createc.cpp.i

CMakeFiles/createc.dir/src/createc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/createc.dir/src/createc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/src/createc.cpp -o CMakeFiles/createc.dir/src/createc.cpp.s

# Object files for target createc
createc_OBJECTS = \
"CMakeFiles/createc.dir/src/createc.cpp.o"

# External object files for target createc
createc_EXTERNAL_OBJECTS =

createc: CMakeFiles/createc.dir/src/createc.cpp.o
createc: CMakeFiles/createc.dir/build.make
createc: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.8
createc: CMakeFiles/createc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable createc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/createc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/createc.dir/build: createc
.PHONY : CMakeFiles/createc.dir/build

CMakeFiles/createc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/createc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/createc.dir/clean

CMakeFiles/createc.dir/depend:
	cd /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build /media/debos/226AD3E96AD3B7AD/CPP/tools/CreateC/build/CMakeFiles/createc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/createc.dir/depend

