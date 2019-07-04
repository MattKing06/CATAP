message( "Processing Hardware source files...")
set( SOURCE_FILES
    source/Hardware.cpp
)

set( INCLUDE_FILES
    include/Hardware.h
)

set( TEST_FILES
	HardwareTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Hardware STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Hardware PUBLIC ${Boost_INCLUDE_DIR})
#EPICS RELATED VARS
target_include_directories(Hardware PUBLIC ${EPICS_DIR}/include)
target_include_directories(Hardware PUBLIC ${EPICS_OS_INCLUDES})
TARGET_LINK_LIBRARIES(Hardware PUBLIC ${EPICS_COM_LIB} ${EPICS_CA_LIB} PV ConfigReader LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
include_directories(include)
add_subdirectory(test)
