# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\simor\Programming Files\backtester"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\simor\Programming Files\backtester\build"

# Include any dependencies generated for this target.
include libs/A/CMakeFiles/A.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/A/CMakeFiles/A.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/A/CMakeFiles/A.dir/progress.make

# Include the compile flags for this target's objects.
include libs/A/CMakeFiles/A.dir/flags.make

libs/A/CMakeFiles/A.dir/src/A.cpp.obj: libs/A/CMakeFiles/A.dir/flags.make
libs/A/CMakeFiles/A.dir/src/A.cpp.obj: libs/A/CMakeFiles/A.dir/includes_CXX.rsp
libs/A/CMakeFiles/A.dir/src/A.cpp.obj: C:/Users/simor/Programming\ Files/backtester/libs/A/src/A.cpp
libs/A/CMakeFiles/A.dir/src/A.cpp.obj: libs/A/CMakeFiles/A.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\simor\Programming Files\backtester\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/A/CMakeFiles/A.dir/src/A.cpp.obj"
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/A/CMakeFiles/A.dir/src/A.cpp.obj -MF CMakeFiles\A.dir\src\A.cpp.obj.d -o CMakeFiles\A.dir\src\A.cpp.obj -c "C:\Users\simor\Programming Files\backtester\libs\A\src\A.cpp"

libs/A/CMakeFiles/A.dir/src/A.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A.dir/src/A.cpp.i"
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\simor\Programming Files\backtester\libs\A\src\A.cpp" > CMakeFiles\A.dir\src\A.cpp.i

libs/A/CMakeFiles/A.dir/src/A.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A.dir/src/A.cpp.s"
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\simor\Programming Files\backtester\libs\A\src\A.cpp" -o CMakeFiles\A.dir\src\A.cpp.s

# Object files for target A
A_OBJECTS = \
"CMakeFiles/A.dir/src/A.cpp.obj"

# External object files for target A
A_EXTERNAL_OBJECTS =

libs/A/libA.a: libs/A/CMakeFiles/A.dir/src/A.cpp.obj
libs/A/libA.a: libs/A/CMakeFiles/A.dir/build.make
libs/A/libA.a: libs/A/CMakeFiles/A.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\simor\Programming Files\backtester\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libA.a"
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && $(CMAKE_COMMAND) -P CMakeFiles\A.dir\cmake_clean_target.cmake
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\A.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/A/CMakeFiles/A.dir/build: libs/A/libA.a
.PHONY : libs/A/CMakeFiles/A.dir/build

libs/A/CMakeFiles/A.dir/clean:
	cd /d C:\Users\simor\PROGRA~1\BACKTE~1\build\libs\A && $(CMAKE_COMMAND) -P CMakeFiles\A.dir\cmake_clean.cmake
.PHONY : libs/A/CMakeFiles/A.dir/clean

libs/A/CMakeFiles/A.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\simor\Programming Files\backtester" "C:\Users\simor\Programming Files\backtester\libs\A" "C:\Users\simor\Programming Files\backtester\build" "C:\Users\simor\Programming Files\backtester\build\libs\A" "C:\Users\simor\Programming Files\backtester\build\libs\A\CMakeFiles\A.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : libs/A/CMakeFiles/A.dir/depend
