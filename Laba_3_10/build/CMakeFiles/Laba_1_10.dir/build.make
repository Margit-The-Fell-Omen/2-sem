# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/tim/sem-2/Laba_3_10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tim/sem-2/Laba_3_10/build

# Include any dependencies generated for this target.
include CMakeFiles/Laba_1_10.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Laba_1_10.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Laba_1_10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Laba_1_10.dir/flags.make

CMakeFiles/Laba_1_10.dir/main.c.o: CMakeFiles/Laba_1_10.dir/flags.make
CMakeFiles/Laba_1_10.dir/main.c.o: /home/tim/sem-2/Laba_3_10/main.c
CMakeFiles/Laba_1_10.dir/main.c.o: CMakeFiles/Laba_1_10.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/tim/sem-2/Laba_3_10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Laba_1_10.dir/main.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Laba_1_10.dir/main.c.o -MF CMakeFiles/Laba_1_10.dir/main.c.o.d -o CMakeFiles/Laba_1_10.dir/main.c.o -c /home/tim/sem-2/Laba_3_10/main.c

CMakeFiles/Laba_1_10.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Laba_1_10.dir/main.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tim/sem-2/Laba_3_10/main.c > CMakeFiles/Laba_1_10.dir/main.c.i

CMakeFiles/Laba_1_10.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Laba_1_10.dir/main.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tim/sem-2/Laba_3_10/main.c -o CMakeFiles/Laba_1_10.dir/main.c.s

# Object files for target Laba_1_10
Laba_1_10_OBJECTS = \
"CMakeFiles/Laba_1_10.dir/main.c.o"

# External object files for target Laba_1_10
Laba_1_10_EXTERNAL_OBJECTS =

Laba_1_10: CMakeFiles/Laba_1_10.dir/main.c.o
Laba_1_10: CMakeFiles/Laba_1_10.dir/build.make
Laba_1_10: libfunctions.a
Laba_1_10: CMakeFiles/Laba_1_10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/tim/sem-2/Laba_3_10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Laba_1_10"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Laba_1_10.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Laba_1_10.dir/build: Laba_1_10
.PHONY : CMakeFiles/Laba_1_10.dir/build

CMakeFiles/Laba_1_10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Laba_1_10.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Laba_1_10.dir/clean

CMakeFiles/Laba_1_10.dir/depend:
	cd /home/tim/sem-2/Laba_3_10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tim/sem-2/Laba_3_10 /home/tim/sem-2/Laba_3_10 /home/tim/sem-2/Laba_3_10/build /home/tim/sem-2/Laba_3_10/build /home/tim/sem-2/Laba_3_10/build/CMakeFiles/Laba_1_10.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Laba_1_10.dir/depend

