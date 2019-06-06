message( "Processing EPICS Interface source files...")
set( SOURCE_FILES
    source/EPICSInterface.cpp
    source/EPICSMagnetInterface.cpp
)

set( INCLUDE_FILES
    include/EPICSInterface.h
    include/EPICSMagnetInterface.h 
)

set( TEST_FILES
	EPICSInterfaceTest.cpp
        EPICSMagnetInterfaceTest.cpp
)
cmake_policy(SET CMP0022 NEW)
add_library(EPICSInterface SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(EPICSInterface PUBLIC ${EPICS_DIR}/include ${EPICS_OS_INCLUDES})
target_include_directories(EPICSInterface PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(EPICSInterface PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(EPICSInterface PUBLIC include)
TARGET_LINK_LIBRARIES(EPICSInterface PUBLIC ${Boost_LIBRARIES} ${EPICS_COM_LIB} ${EPICS_CA_LIB} LoggingSystem)
set_property(TARGET EPICSInterface PROPERTY FOLDER "Magnet")
add_subdirectory(test)
