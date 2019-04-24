message( "Processing source files...")
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
add_library(Hardware ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/Controller/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
TARGET_LINK_LIBRARIES(Hardware PUBLIC ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}Com.lib ${EPICS_LIBRARYDIR}ca.lib ConfigReader LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
include_directories(include)
add_subdirectory(test)