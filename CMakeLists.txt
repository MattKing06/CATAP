cmake_minimum_required(VERSION 2.8)

project(hardware_toy)
cmake_policy(SET CMP0074 NEW)

if(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)

set(PY_VERSION 3)

set(BOOST_ROOT "C:/boost/boost_1_63_0_with_test_built/boost_1_63_0/")

set(EPICS_DIR "C:/epics/base-3.14.12.5")

set(BOOST_LIBRARYDIR "C:/boost/boost_1_63_0_with_test_built/boost_1_63_0/stage/lib")

set(YAML_PARSER_LIB_RELEASE_DIR "C:/yaml-build/Release")
set(YAML_PARSER_LIB_DEBUG_DIR "C:/yaml-build/Debug")
set(YAML_PARSER_DIR "C:/yaml")
set(EPICS_LIBRARYDIR "C:/epics/base-3.14.12.5/lib/win32-x86/")
set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS "Debug;RelWithDebInfo")
set_property(GLOBAL PROPERTY RELEASE_CONFIGURATIONS "Release;MinSizeRel")

message("found epics directory: ${EPICS_DIR}")

include_directories(${EPICS_DIR}/include)

include_directories(${EPICS_DIR}/include/os/WIN32)

find_package(PythonLibs ${PY_VERSION} REQUIRED)

find_package(Boost 1.63 COMPONENTS unit_test_framework filesystem python REQUIRED)

message("Finding Boost...")

if(Boost_FOUND)

    message("Found Boost")

    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${PYTHON_INCLUDE_DIRS})
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

endif(Boost_FOUND)

add_subdirectory(Magnet)
add_subdirectory(HardwareFactory)
add_subdirectory(LoggingSystem)
add_subdirectory(ConfigReader)
add_subdirectory(Hardware)

message(" C++: '${CMAKE_CXX_COMPILER}'")

message("Using CMake version: ${CMAKE_VERSION}")