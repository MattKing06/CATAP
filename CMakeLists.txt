cmake_minimum_required(VERSION 2.8)
project(CATAP)
cmake_policy(SET CMP0074 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CXX_STANDARD 17)

if (WIN32)
  message(STATUS "WIN32 BUILD")
  # These are default versions for CATAP build
  # They can be set here or via cmake-gui
  # Setting here will persist between caches, 
  # setting via gui will change if cache is deleted.
  set(CATAP_PY_VERSION "NOT-SET" CACHE STRING "Python Version")
  set(CATAP_BOOST_VERSION 1.76)
  set(Boost_INCLUDE_DIR "C:/boost/boost_1_76_0")
  IF(NOT ${CATAP_PY_VERSION} MATCHES 3.6)
    IF(NOT ${CATAP_PY_VERSION} MATCHES 3.8)
      message(FATAL_ERROR "Please set the python version you require for CATAP (Ungrouped Entries > CATAP_PY_VERSION).
      Currently we only support versions: 3.6, 3.8 ")
    ENDIF(NOT ${CATAP_PY_VERSION} MATCHES 3.8)
  ENDIF(NOT ${CATAP_PY_VERSION} MATCHES 3.6)
  #########################################
  # Boost 1.76 ONLY BUILT WITH PYTHON 3.8 #
  # and 3.6                               #
  # So we are setting PY VERSION EXPLICIT #
  #########################################
  IF(${CATAP_PY_VERSION} MATCHES 3.8)
  find_package(Boost ${CATAP_BOOST_VERSION} EXACT REQUIRED COMPONENTS python38 system numpy38 filesystem thread chrono atomic date_time unit_test_framework)
  ENDIF(${CATAP_PY_VERSION} MATCHES 3.8)
  IF(${CATAP_PY_VERSION} MATCHES 3.6)
  find_package(Boost ${CATAP_BOOST_VERSION} EXACT REQUIRED COMPONENTS python36 system numpy filesystem thread chrono atomic date_time unit_test_framework)
  ENDIF(${CATAP_PY_VERSION} MATCHES 3.6)
  message("PYTHON 3.8 BUILD: ")
  find_package(Python3 ${CATAP_PY_VERSION} EXACT COMPONENTS Development)
  message("include: ${Python3_INCLUDE_DIRS}")
  message("libs: ${Python3_LIBRARIES}")

  set(DEPLOY_CATAP "not set"  CACHE STRING "Build for deployment")
  if (${DEPLOY_CATAP} MATCHES "not set")
        message(FATAL_ERROR "Please set whether you are building CATAP for deployment. ")
  endif(${DEPLOY_CATAP} MATCHES "not set")

  if (${DEPLOY_CATAP} MATCHES "false")
    set(MASTER_LATTICE_LOCATION "${CMAKE_SOURCE_DIR}/MasterLattice")
    add_definitions(-DMASTER_LATTICE_LOCATION="${MASTER_LATTICE_LOCATION}")
  endif(${DEPLOY_CATAP} MATCHES "false")

  if (${DEPLOY_CATAP} MATCHES "true")
   set(DEPLOY_MASTER_LATTICE_LOCATION "NOT-SET" CACHE STRING "Location for ML to be read from in deployment")
   if (${DEPLOY_MASTER_LATTICE_LOCATION} MATCHES "NOT-SET")
     message(FATAL_ERROR "Please set where you are building CATAP for deployment. The lattice definitions will be moved there.")
   endif(${DEPLOY_MASTER_LATTICE_LOCATION} MATCHES "NOT-SET")
   set(MASTER_LATTICE_LOCATION ${DEPLOY_MASTER_LATTICE_LOCATION})
   message("Setting Deploy Master Lattice Location:" ${MASTER_LATTICE_LOCATION} )
   add_definitions(-DMASTER_LATTICE_LOCATION="${MASTER_LATTICE_LOCATION}")
  endif(${DEPLOY_CATAP} MATCHES "true")

  set(EPICS_DIR "C:/epics/base-3.14.12.5")
  set(YAML_PARSER_LIB_RELEASE_DIR "C:/yaml-cpp-build/Release")
  set(YAML_PARSER_LIB_DEBUG_DIR "C:/yaml-cpp-build/Debug")
  set(YAML_PARSER_DIR "C:/yaml")
  set(EPICS_LIBRARYDIR "C:/epics/base-3.14.12.5/lib/windows-x64/")
  set(EPICS_DLLDIR "C:/epics/base-3.14.12.5/bin/windows-x64/")
  set(EPICS_OS_INCLUDES ${EPICS_DIR}/include/os/WIN32)
  set(EPICS_COM_LIB ${EPICS_LIBRARYDIR}Com.lib)
  set(EPICS_CA_LIB ${EPICS_LIBRARYDIR}ca.lib)
  set(EPICS_COM_DLL ${EPICS_DLLDIR}Com.dll)
  set(EPICS_CA_DLL ${EPICS_DLLDIR}ca.dll)
  set(THIRD_PARTY_LIBS ${CMAKE_SOURCE_DIR}/thirdparty)
endif(WIN32)
if(UNIX)
  message(STATUS "UNIX BUILD")
  set(BOOSTROOT "~/dev/boost_1_76_0")
  set(EPICS_DIR "~/dev/base-3.14.12.5/")
  set(YAML_PARSER_LIB_DIR "/usr/local/lib")
  set(YAML_PARSER_DIR "~/dev/yaml-cpp")
  set(EPICS_LIBRARYDIR "~/dev/base-3.14.12.5/lib/linux-x86_64/")
  set(EPICS_OS_INCLUDES ${EPICS_DIR}/include/os/Linux)
  set(EPICS_COM_LIB ${EPICS_LIBRARYDIR}libCom.so)
  set(EPICS_CA_LIB ${EPICS_LIBRARYDIR}libca.so)
  find_package(Boost 1.76 EXACT REQUIRED COMPONENTS python38 numpy38 system filesystem thread chrono atomic date_time unit_test_framework)
  find_package(Python3 3.8 EXACT COMPONENTS Development)
  set(MASTER_LATTICE_LOCATION "${CMAKE_SOURCE_DIR}/MasterLattice")
  add_definitions(-DMASTER_LATTICE_LOCATION="${MASTER_LATTICE_LOCATION}")
endif(UNIX)

set_property(GLOBAL PROPERTY DEBUG_COnFIGURATIOnS "Debug;RelWithDebInfo")
set_property(GLOBAL PROPERTY RELEASE_COnFIGURATIOnS "Release;MinSizeRel")



message("Finding Boost...")

if(Boost_FOUND)
    message("Found Boost")
    add_definitiOns(${Boost_LIB_DIAGNOSTIC_DEFINITIOnS})
    if(WIN32)
      message(STATUS "WIN32 BUILD: TRUE")
      set(Boost_USE_STATIC_LIBS Off)
      set(Boost_USE_SHARED_LIBS On)
      set(Boost_USE_MULTITHREAD On)
      set(Boost_USE_STATIC_RUNTIME Off)
      add_definitiOns(-DBOOST_ALL_DYN_LINK)
    endif(WIN32)
    if(UNIX)
      set(Boost_USE_STATIC_LIBS Off)
      set(Boost_USE_SHARED_LIBS On)
      set(Boost_USE_MULTITHREAD On)
      set(Boost_USE_STATIC_RUNTIME Off)
      add_definitiOns(-DBOOST_ALL_DYN_LINK)
    endif(UNIX)
endif(Boost_FOUND)



add_definitions(-DHAVE_SNPRINTF)



add_subdirectory(Factories)
add_subdirectory(LoggingSystem)
add_subdirectory(ConfigReader)
add_subdirectory(Hardware)
add_subdirectory(PythonInterface)
add_subdirectory(PV)
add_subdirectory(EPICSInterface)
add_subdirectory(EPICSTools)
add_subdirectory(Utilities)
add_subdirectory(Docs)
add_subdirectory(HighLevelSystems)
