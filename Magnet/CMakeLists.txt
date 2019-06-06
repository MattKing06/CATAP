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
add_library(Magnet SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Magnet PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Magnet PUBLIC include)
TARGET_LINK_LIBRARIES(Magnet PUBLIC ${Boost_LIBRARIES} ${EPICS_COM_LIB} ${EPICS_CA_LIB} Hardware EPICSInterface)
set_property(TARGET Magnet PROPERTY FOLDER "Magnet")
add_subdirectory(test)
