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
CMAKE_SOURCE_DIR = /Users/joelcummings/Developer/A2TestHarness

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/joelcummings/Developer/A2TestHarness/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/A1TestHarness.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/A1TestHarness.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/A1TestHarness.dir/flags.make

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o: ../src/calMemTest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/calMemTest.c

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/calMemTest.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/calMemTest.c > CMakeFiles/A1TestHarness.dir/src/calMemTest.c.i

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/calMemTest.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/calMemTest.c -o CMakeFiles/A1TestHarness.dir/src/calMemTest.c.s

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.requires

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.provides: CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.provides

CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o


CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o: ../src/calMemTestPrint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/calMemTestPrint.c

CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/calMemTestPrint.c > CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.i

CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/calMemTestPrint.c -o CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.s

CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.requires

CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.provides: CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.provides

CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o


CMakeFiles/A1TestHarness.dir/src/calTest.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/calTest.c.o: ../src/calTest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/A1TestHarness.dir/src/calTest.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/calTest.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/calTest.c

CMakeFiles/A1TestHarness.dir/src/calTest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/calTest.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/calTest.c > CMakeFiles/A1TestHarness.dir/src/calTest.c.i

CMakeFiles/A1TestHarness.dir/src/calTest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/calTest.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/calTest.c -o CMakeFiles/A1TestHarness.dir/src/calTest.c.s

CMakeFiles/A1TestHarness.dir/src/calTest.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/calTest.c.o.requires

CMakeFiles/A1TestHarness.dir/src/calTest.c.o.provides: CMakeFiles/A1TestHarness.dir/src/calTest.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/calTest.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/calTest.c.o.provides

CMakeFiles/A1TestHarness.dir/src/calTest.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/calTest.c.o


CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o: ../src/calTestCases.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/calTestCases.c

CMakeFiles/A1TestHarness.dir/src/calTestCases.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/calTestCases.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/calTestCases.c > CMakeFiles/A1TestHarness.dir/src/calTestCases.c.i

CMakeFiles/A1TestHarness.dir/src/calTestCases.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/calTestCases.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/calTestCases.c -o CMakeFiles/A1TestHarness.dir/src/calTestCases.c.s

CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.requires

CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.provides: CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.provides

CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o


CMakeFiles/A1TestHarness.dir/src/listTest.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/listTest.c.o: ../src/listTest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/A1TestHarness.dir/src/listTest.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/listTest.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/listTest.c

CMakeFiles/A1TestHarness.dir/src/listTest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/listTest.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/listTest.c > CMakeFiles/A1TestHarness.dir/src/listTest.c.i

CMakeFiles/A1TestHarness.dir/src/listTest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/listTest.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/listTest.c -o CMakeFiles/A1TestHarness.dir/src/listTest.c.s

CMakeFiles/A1TestHarness.dir/src/listTest.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/listTest.c.o.requires

CMakeFiles/A1TestHarness.dir/src/listTest.c.o.provides: CMakeFiles/A1TestHarness.dir/src/listTest.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/listTest.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/listTest.c.o.provides

CMakeFiles/A1TestHarness.dir/src/listTest.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/listTest.c.o


CMakeFiles/A1TestHarness.dir/src/testCases.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/testCases.c.o: ../src/testCases.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/A1TestHarness.dir/src/testCases.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/testCases.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/testCases.c

CMakeFiles/A1TestHarness.dir/src/testCases.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/testCases.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/testCases.c > CMakeFiles/A1TestHarness.dir/src/testCases.c.i

CMakeFiles/A1TestHarness.dir/src/testCases.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/testCases.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/testCases.c -o CMakeFiles/A1TestHarness.dir/src/testCases.c.s

CMakeFiles/A1TestHarness.dir/src/testCases.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/testCases.c.o.requires

CMakeFiles/A1TestHarness.dir/src/testCases.c.o.provides: CMakeFiles/A1TestHarness.dir/src/testCases.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/testCases.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/testCases.c.o.provides

CMakeFiles/A1TestHarness.dir/src/testCases.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/testCases.c.o


CMakeFiles/A1TestHarness.dir/src/testharness.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/src/testharness.c.o: ../src/testharness.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/A1TestHarness.dir/src/testharness.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/src/testharness.c.o   -c /Users/joelcummings/Developer/A2TestHarness/src/testharness.c

CMakeFiles/A1TestHarness.dir/src/testharness.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/src/testharness.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/src/testharness.c > CMakeFiles/A1TestHarness.dir/src/testharness.c.i

CMakeFiles/A1TestHarness.dir/src/testharness.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/src/testharness.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/src/testharness.c -o CMakeFiles/A1TestHarness.dir/src/testharness.c.s

CMakeFiles/A1TestHarness.dir/src/testharness.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/src/testharness.c.o.requires

CMakeFiles/A1TestHarness.dir/src/testharness.c.o.provides: CMakeFiles/A1TestHarness.dir/src/testharness.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/src/testharness.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/src/testharness.c.o.provides

CMakeFiles/A1TestHarness.dir/src/testharness.c.o.provides.build: CMakeFiles/A1TestHarness.dir/src/testharness.c.o


CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o: ../testFiles/invCalendar/lineEnder.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o   -c /Users/joelcummings/Developer/A2TestHarness/testFiles/invCalendar/lineEnder.c

CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/testFiles/invCalendar/lineEnder.c > CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.i

CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/testFiles/invCalendar/lineEnder.c -o CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.s

CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.requires

CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.provides: CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.provides

CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.provides.build: CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o


CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o: CMakeFiles/A1TestHarness.dir/flags.make
CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o: ../testFiles/validCalendar/lineEnder.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o   -c /Users/joelcummings/Developer/A2TestHarness/testFiles/validCalendar/lineEnder.c

CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joelcummings/Developer/A2TestHarness/testFiles/validCalendar/lineEnder.c > CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.i

CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joelcummings/Developer/A2TestHarness/testFiles/validCalendar/lineEnder.c -o CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.s

CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.requires:

.PHONY : CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.requires

CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.provides: CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.requires
	$(MAKE) -f CMakeFiles/A1TestHarness.dir/build.make CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.provides.build
.PHONY : CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.provides

CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.provides.build: CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o


# Object files for target A1TestHarness
A1TestHarness_OBJECTS = \
"CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o" \
"CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o" \
"CMakeFiles/A1TestHarness.dir/src/calTest.c.o" \
"CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o" \
"CMakeFiles/A1TestHarness.dir/src/listTest.c.o" \
"CMakeFiles/A1TestHarness.dir/src/testCases.c.o" \
"CMakeFiles/A1TestHarness.dir/src/testharness.c.o" \
"CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o" \
"CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o"

# External object files for target A1TestHarness
A1TestHarness_EXTERNAL_OBJECTS =

A1TestHarness: CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/calTest.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/listTest.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/testCases.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/src/testharness.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o
A1TestHarness: CMakeFiles/A1TestHarness.dir/build.make
A1TestHarness: CMakeFiles/A1TestHarness.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C executable A1TestHarness"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/A1TestHarness.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/A1TestHarness.dir/build: A1TestHarness

.PHONY : CMakeFiles/A1TestHarness.dir/build

CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/calMemTest.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/calMemTestPrint.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/calTest.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/calTestCases.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/listTest.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/testCases.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/src/testharness.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/testFiles/invCalendar/lineEnder.c.o.requires
CMakeFiles/A1TestHarness.dir/requires: CMakeFiles/A1TestHarness.dir/testFiles/validCalendar/lineEnder.c.o.requires

.PHONY : CMakeFiles/A1TestHarness.dir/requires

CMakeFiles/A1TestHarness.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/A1TestHarness.dir/cmake_clean.cmake
.PHONY : CMakeFiles/A1TestHarness.dir/clean

CMakeFiles/A1TestHarness.dir/depend:
	cd /Users/joelcummings/Developer/A2TestHarness/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/joelcummings/Developer/A2TestHarness /Users/joelcummings/Developer/A2TestHarness /Users/joelcummings/Developer/A2TestHarness/cmake-build-debug /Users/joelcummings/Developer/A2TestHarness/cmake-build-debug /Users/joelcummings/Developer/A2TestHarness/cmake-build-debug/CMakeFiles/A1TestHarness.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/A1TestHarness.dir/depend

