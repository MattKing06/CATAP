message( "Processing Magnet source files...")
set( SOURCE_FILES
    source/Magnet.cpp
)

set( INCLUDE_FILES
    include/Magnet.h 
)

set( TEST_FILES
	MagnetTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Magnet STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Magnet PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Magnet PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Magnet PUBLIC include)
TARGET_LINK_LIBRARIES(Magnet PUBLIC Hardware PV EPICSInterface)
set_property(TARGET Magnet PROPERTY FOLDER "Magnet")
add_subdirectory(test)
