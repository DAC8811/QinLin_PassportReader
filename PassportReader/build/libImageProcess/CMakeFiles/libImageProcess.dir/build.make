# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gwi/桌面/passport/QiLin_WI_A/PassportReader

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build

# Include any dependencies generated for this target.
include libImageProcess/CMakeFiles/libImageProcess.dir/depend.make

# Include the progress variables for this target.
include libImageProcess/CMakeFiles/libImageProcess.dir/progress.make

# Include the compile flags for this target's objects.
include libImageProcess/CMakeFiles/libImageProcess.dir/flags.make

libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o: libImageProcess/CMakeFiles/libImageProcess.dir/flags.make
libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o: ../libImageProcess/src/ImageProcess.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o"
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o -c /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/libImageProcess/src/ImageProcess.cpp

libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.i"
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/libImageProcess/src/ImageProcess.cpp > CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.i

libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.s"
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/libImageProcess/src/ImageProcess.cpp -o CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.s

# Object files for target libImageProcess
libImageProcess_OBJECTS = \
"CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o"

# External object files for target libImageProcess
libImageProcess_EXTERNAL_OBJECTS =

libImageProcess/liblibImageProcess.a: libImageProcess/CMakeFiles/libImageProcess.dir/src/ImageProcess.cpp.o
libImageProcess/liblibImageProcess.a: libImageProcess/CMakeFiles/libImageProcess.dir/build.make
libImageProcess/liblibImageProcess.a: libImageProcess/CMakeFiles/libImageProcess.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblibImageProcess.a"
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && $(CMAKE_COMMAND) -P CMakeFiles/libImageProcess.dir/cmake_clean_target.cmake
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libImageProcess.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libImageProcess/CMakeFiles/libImageProcess.dir/build: libImageProcess/liblibImageProcess.a

.PHONY : libImageProcess/CMakeFiles/libImageProcess.dir/build

libImageProcess/CMakeFiles/libImageProcess.dir/clean:
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess && $(CMAKE_COMMAND) -P CMakeFiles/libImageProcess.dir/cmake_clean.cmake
.PHONY : libImageProcess/CMakeFiles/libImageProcess.dir/clean

libImageProcess/CMakeFiles/libImageProcess.dir/depend:
	cd /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gwi/桌面/passport/QiLin_WI_A/PassportReader /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/libImageProcess /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess /home/gwi/桌面/passport/QiLin_WI_A/PassportReader/build/libImageProcess/CMakeFiles/libImageProcess.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libImageProcess/CMakeFiles/libImageProcess.dir/depend

