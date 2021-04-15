# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# move cxx flags to build.gradle
# -fopenmp will cause random white lines in pics
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -std=gnu++17 -fexceptions -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -std=gnu++11 -fexceptions -frtti -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=gnu++11 -fexceptions -frtti")

#=========================================================
# pre-build static libs

# opencv
# opencv build command:
# /Users/ooxx/Library/Android/sdk/cmake/3.6.4111459/bin/cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/ooxx/Library/Android/sdk/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_NDK=/Users/ooxx/Library/Android/sdk/ndk-bundle -DANDROID_NATIVE_API_LEVEL=android-22 -DBUILD_JAVA=OFF -DBUILD_ANDROID_EXAMPLES=OFF -DBUILD_ANDROID_PROJECTS=OFF -DANDROID_STL=c++_shared -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX:PATH=/Users/ooxx/AndroidStudioProjects/PassportReader/app/src/main/3rdparty -DANDROID_ABI=armeabi-v7a -DCMAKE_CXX_FLAGS_RELEASE="-O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -std=gnu++11 -fexceptions -frtti -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17" -DCMAKE_C_FLAGS_RELEASE="-O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -frtti -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17" -DBUILD_opencv_dnn=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DENABLE_VFPV3=ON -DENABLE_NEON=ON -DCPU_BASELINE_DISABLE="" -DBUILD_PNG=OFF -DWITH_PNG=OFF
set(3rdparty_lib ${CMAKE_SOURCE_DIR}/src/main/3rdparty)
#set(OpenCV_DIR ${3rdparty_lib}/sdk/native/jni)
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
add_library(opencv SHARED IMPORTED)
set_target_properties(opencv PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libopencv_java4.so")
include_directories(${3rdparty_lib}/sdk/native/jni/include)

#openjpeg build command:
#/Users/ooxx/Library/Android/sdk/cmake/3.6.4111459/bin/cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/ooxx/Library/Android/sdk/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_NDK=/Users/ooxx/Library/Android/sdk/ndk-bundle -DANDROID_NATIVE_API_LEVEL=android-22 -DANDROID_STL=c++_static -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX:PATH=/Users/ooxx/AndroidStudioProjects/PassportReader/app/src/main/3rdparty -DANDROID_ABI=armeabi-v7a -DCMAKE_CXX_FLAGS_RELEASE="-O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -std=gnu++11 -fexceptions -frtti -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17" -DCMAKE_C_FLAGS_RELEASE="-O3 -Wall -Wextra -ffast-math -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize -frtti -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a17"
set(OpenJPEG_DIR ${3rdparty_lib}/lib/openjpeg-2.3/)
find_package(OpenJPEG REQUIRED)

#build script:
#https://github.com/willyliu/build-openssl-android
add_library(crypto STATIC IMPORTED)
set_target_properties(crypto PROPERTIES
        IMPORTED_LOCATION "${3rdparty_lib}/lib/libcrypto.a")

#libusb build command:
#/Users/ooxx/Library/Android/sdk/ndk-bundle/ndk-build APP_BUILD_SCRIPT=`pwd`/android/jni/Android.mk NDK_PROJECT_PATH=`pwd` APP_ALLOW_MISSING_DEPS=true APP_CFLAGS="-Wall -Wextra -fexceptions -frtti -fPIC -O3 -ffast-math -funsafe-math-optimizations -mfloat-abi=softfp -mfpu=neon-vfpv4 " APP_CPPFLAGS="-Wall -Wextra -std=gnu++14 -fexceptions -frtti -fPIC -O3 -ffast-math -funsafe-math-optimizations -mfloat-abi=softfp -mfpu=neon-vfpv4 " APP_PLATFORM=android-22 APP_MODULES=usb1.0
add_library(usb-1.0 SHARED IMPORTED)
set_target_properties(usb-1.0 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libusb1.0.so")

include_directories(${3rdparty_lib}/include/)

#我自己加的日志库
add_library(gflags STATIC IMPORTED)
set_target_properties(gflags PROPERTIES
        IMPORTED_LOCATION "${3rdparty_lib}/lib/libgflags.a")
add_library(glog STATIC IMPORTED)
set_target_properties(glog PROPERTIES
        IMPORTED_LOCATION "${3rdparty_lib}/lib/libglog.a")

#=========================================================
# pre-build shared libs
set(shared_libs_dir "${CMAKE_SOURCE_DIR}/src/main/jniLibs")

#download from http://www.mindvision.com.cn/rjxz/list_12.aspx?lcid=138

#add_library(MVSDK SHARED IMPORTED)
#set_target_properties(MVSDK PROPERTIES
#        IMPORTED_LOCATION "${shared_libs_dir}/${ANDROID_ABI}/libmvsdk.so")

#=========================================================
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        PassportReaderJNI

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/PassportReaderJNI.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


file(GLOB header_files
        "${CMAKE_SOURCE_DIR}/src/main/cpp/PassportReader/*/include/"
        )

include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
include_directories(${api_header_files})

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        PassportReaderJNI

        getData

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/PassportReader)
