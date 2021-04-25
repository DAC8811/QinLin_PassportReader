cmake_minimum_required(VERSION 3.4.1)
project (PassportReaderC)

set(CMAKE_CXX_STANDARD 14)

set(api_header_files
        "${CMAKE_CURRENT_SOURCE_DIR}/PassportReaderAPI/include/"
		"${CMAKE_CURRENT_SOURCE_DIR}/PassportReaderAPI/include/json/")



message("================================================================")
message("Building with ${CMAKE_SYSTEM_NAME} and ${CMAKE_SYSTEM_PROCESSOR}")

# 根据不同操作系统，选择不同的编译选项
if (ANDROID_TOOLCHAIN)
    #add_compile_definitions(__ANDROID__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ANDROID__")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__ANDROID__")
    file(GLOB 3rdparty_header_files
            "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include/"
            )
    include_directories(${3rdparty_header_files} ${api_header_files})
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -fPIC -DNDEBUG -O3 -ffast-math -ftree-vectorize -fno-lto -fexceptions -fdata-sections -ffunction-sections -Wl,--gc-sections ")#
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -fPIC -DNDEBUG  -O3 -ffast-math -ftree-vectorize -fno-lto -fdata-sections -ffunction-sections -Wl,--gc-sections ")# 
    find_package(OpenCV 4 REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(OpenJPEG REQUIRED)
    #find_path(LIBUSB_INCLUDE_DIR
    #    NAMES libusb.h
    #    PATH_SUFFIXES "include" "libusb" "libusb-1.0")
 	add_library(usb-1.0 STATIC IMPORTED)
    set_target_properties(usb-1.0 PROPERTIES
    	IMPORTED_LOCATION "${CMAKE_FIND_ROOT_PATH}/lib/libusb-1.0.a")
	add_library(crypto STATIC IMPORTED)
    set_target_properties(crypto PROPERTIES
    	IMPORTED_LOCATION "${CMAKE_FIND_ROOT_PATH}/lib/libcrypto.a")
	add_library(glog STATIC IMPORTED)
    set_target_properties(glog PROPERTIES
    	IMPORTED_LOCATION "${CMAKE_FIND_ROOT_PATH}/lib/libglog.a")
	add_library(libwlt SHARED IMPORTED)
	set_target_properties(libwlt PROPERTIES
    	IMPORTED_LOCATION "${CMAKE_FIND_ROOT_PATH}/lib/libwlt2019.so")


	#file(GLOB cardRecog_header_files
    #        "${CMAKE_CURRENT_SOURCE_DIR}/cardRecog1.0.0/src"
    #        )
	add_library(libcardRecog STATIC IMPORTED)
	set_target_properties(libcardRecog PROPERTIES
    	IMPORTED_LOCATION "${CMAKE_FIND_ROOT_PATH}/lib/libcardRecog.a")

	#tensorflow加载
	#set(tf_ROOT "${CMAKE_FIND_ROOT_PATH}/include/tensorflow")
	#set(tf_INCLUDE_DIRS ${tf_ROOT}
    #                	${tf_ROOT}/bazel-genfiles
    #                	${tf_ROOT}/bazel-bin/tensorflow)
	#set(tf_LIBRARIES ${CMAKE_FIND_ROOT_PATH}/lib/tensorflow/libtensorflow_cc.so  ${CMAKE_FIND_ROOT_PATH}/lib/tensorflow/libtensorflow_framework.so)

	#set(protobuf_INCLUDE_DIRS ${CMAKE_FIND_ROOT_PATH}/thirdParty/protobuf/include)
	#set(protobuf_LIBRARIES ${CMAKE_FIND_ROOT_PATH}/thirdParty/protobuf/lib/libprotobuf.a)

	#set(eigen3_INCLUDE_DIRS ${CMAKE_FIND_ROOT_PATH}/thirdParty/eigen3)
	
	#add_library(tensorflow SHARED IMPORTED)
	#set_target_properties(tensorflow PROPERTIES 
	#	IMPORTED_LOCATION 
	#	"${CMAKE_FIND_ROOT_PATH}/lib/tensorflow/libtensorflow_cc.so ${CMAKE_FIND_ROOT_PATH}/lib/tensorflow/libtensorflow_framework.so")
	#tensorflow加载完毕
	include_directories(${OpenCV_INCLUDE_DIRS} ${LIBUSB_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${GLOG_INCLUDE_DIR} ${api_header_files} ${OPENJPEG_INCLUDE_DIRS} ${cardRecog_header_files})
	
	

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows" )
        add_compile_definitions(__WINDOWS__)
    else()
        if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch" )
            add_compile_definitions(__LINUX_AARCH__)
            message (" building on aarch, disable things to make it faster ")
            set(USE_OLD_OCR TRUE)
            set(BUILD_WITH_IDR220S TRUE)
        elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" )
            add_compile_definitions(__LINUX_AARCH64__)
            # message (" building on x86-64, use est100 to decode sfz image ")
            # set(BUILD_WITH_EST100 TRUE)
        elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
            add_compile_definitions(__LINUX_X86_64__)
            # message (" building on x86-64, use est100 to decode sfz image ")
            # set(BUILD_WITH_EST100 TRUE)
        endif()
    endif()
endif ()

message("CXXFLAGS ${CMAKE_CXX_FLAGS}")
message("CFLAGS ${CMAKE_C_FLAGS}")

add_subdirectory(libLED)
#add_subdirectory(libCamera)
add_subdirectory(libChip_auth)
add_subdirectory(libImageProcess)
add_subdirectory(libCheck)
if (USE_OLD_OCR)
    message (" building with old ocr lib ")
    add_compile_definitions(USE_OLD_OCR)
    add_subdirectory(libOCR.old)
else()
    message (" building with new CNN ocr lib ")
    #add_subdirectory(libOCR)
    #add_subdirectory(libOCRNew)
	#add_subdirectory(cardRecog1.0.0)不能在这里直接编译，会出问题
endif()
#add_subdirectory(libCheck)
add_subdirectory(PassportReaderAPI)
if (NOT ANDROID_TOOLCHAIN)
#    add_subdirectory(PassportReaderDEMO)
endif ()
