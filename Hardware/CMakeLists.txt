message( "Processing Hardware source files...")
set( SOURCE_FILES
    source/Hardware.cpp
    source/Magnet.cpp
)

set( INCLUDE_FILES
    include/Hardware.h
    include/Magnet.h
)

set( TEST_FILES
	HardwareTest.cpp
	MagnetTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Hardware STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Hardware PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Hardware PUBLIC include)
target_include_directories(Hardware PUBLIC ${Boost_INCLUDE_DIR})
#EPICS RELATED VARS
target_include_directories(Hardware PUBLIC ${EPICS_DIR}/include)
target_include_directories(Hardware PUBLIC ${EPICS_OS_INCLUDES})
TARGET_LINK_LIBRARIES(Hardware PUBLIC ${EPICS_COM_LIB} ${EPICS_CA_LIB} EPICSInterface PV ConfigReader LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
include_directories(include)
add_subdirectory(test)
