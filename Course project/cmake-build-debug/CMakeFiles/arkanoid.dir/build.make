# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\79028\Desktop\EVM\Course project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/arkanoid.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/arkanoid.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/arkanoid.dir/flags.make

CMakeFiles/arkanoid.dir/sources/main.cpp.obj: CMakeFiles/arkanoid.dir/flags.make
CMakeFiles/arkanoid.dir/sources/main.cpp.obj: CMakeFiles/arkanoid.dir/includes_CXX.rsp
CMakeFiles/arkanoid.dir/sources/main.cpp.obj: ../sources/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/arkanoid.dir/sources/main.cpp.obj"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\arkanoid.dir\sources\main.cpp.obj -c "C:\Users\79028\Desktop\EVM\Course project\sources\main.cpp"

CMakeFiles/arkanoid.dir/sources/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arkanoid.dir/sources/main.cpp.i"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\79028\Desktop\EVM\Course project\sources\main.cpp" > CMakeFiles\arkanoid.dir\sources\main.cpp.i

CMakeFiles/arkanoid.dir/sources/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arkanoid.dir/sources/main.cpp.s"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\79028\Desktop\EVM\Course project\sources\main.cpp" -o CMakeFiles\arkanoid.dir\sources\main.cpp.s

CMakeFiles/arkanoid.dir/sources/timer.cpp.obj: CMakeFiles/arkanoid.dir/flags.make
CMakeFiles/arkanoid.dir/sources/timer.cpp.obj: CMakeFiles/arkanoid.dir/includes_CXX.rsp
CMakeFiles/arkanoid.dir/sources/timer.cpp.obj: ../sources/timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/arkanoid.dir/sources/timer.cpp.obj"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\arkanoid.dir\sources\timer.cpp.obj -c "C:\Users\79028\Desktop\EVM\Course project\sources\timer.cpp"

CMakeFiles/arkanoid.dir/sources/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arkanoid.dir/sources/timer.cpp.i"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\79028\Desktop\EVM\Course project\sources\timer.cpp" > CMakeFiles\arkanoid.dir\sources\timer.cpp.i

CMakeFiles/arkanoid.dir/sources/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arkanoid.dir/sources/timer.cpp.s"
	C:\PROGRA~2\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\79028\Desktop\EVM\Course project\sources\timer.cpp" -o CMakeFiles\arkanoid.dir\sources\timer.cpp.s

# Object files for target arkanoid
arkanoid_OBJECTS = \
"CMakeFiles/arkanoid.dir/sources/main.cpp.obj" \
"CMakeFiles/arkanoid.dir/sources/timer.cpp.obj"

# External object files for target arkanoid
arkanoid_EXTERNAL_OBJECTS =

arkanoid.exe: CMakeFiles/arkanoid.dir/sources/main.cpp.obj
arkanoid.exe: CMakeFiles/arkanoid.dir/sources/timer.cpp.obj
arkanoid.exe: CMakeFiles/arkanoid.dir/build.make
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_dnn430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_gapi430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_highgui430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_ml430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_objdetect430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_photo430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_stitching430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_video430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_videoio430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_imgcodecs430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_calib3d430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_features2d430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_flann430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_imgproc430.dll.a
arkanoid.exe: C:/OpenCV2/build/install/x64/mingw/lib/libopencv_core430.dll.a
arkanoid.exe: CMakeFiles/arkanoid.dir/linklibs.rsp
arkanoid.exe: CMakeFiles/arkanoid.dir/objects1.rsp
arkanoid.exe: CMakeFiles/arkanoid.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable arkanoid.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\arkanoid.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/arkanoid.dir/build: arkanoid.exe

.PHONY : CMakeFiles/arkanoid.dir/build

CMakeFiles/arkanoid.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\arkanoid.dir\cmake_clean.cmake
.PHONY : CMakeFiles/arkanoid.dir/clean

CMakeFiles/arkanoid.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\79028\Desktop\EVM\Course project" "C:\Users\79028\Desktop\EVM\Course project" "C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug" "C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug" "C:\Users\79028\Desktop\EVM\Course project\cmake-build-debug\CMakeFiles\arkanoid.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/arkanoid.dir/depend

