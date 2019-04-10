message( "Processing source files...")
set( SOURCE_FILES
    source/EPICSInterface.cpp 
    source/EPICSMagnetInterface.cpp 
    source/MagnetController.cpp 
    source/MagnetPython.cpp 
    source/main.cpp
)

set( INCLUDE_FILES
    include/EPICSInterface.h
    include/EPICSMagnetInterface.h
    include/MagnetController.h 
    include/MagnetPython.h 
)

set( TEST_FILES
	EPICSInterfaceTest.cpp
	EPICSMagnetInterfaceTest.cpp
	MagnetControllerTest.cpp
	MagnetPythonTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Magnet ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/Controller/include)
TARGET_LINK_LIBRARIES(Magnet [${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}Com.lib ${EPICS_LIBRARYDIR}ca.lib])
set_property(TARGET Magnet PROPERTY FOLDER "Magnet")

include_directories(include)
add_subdirectory(test)