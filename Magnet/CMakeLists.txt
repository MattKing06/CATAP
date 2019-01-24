message( "Processing source files...")
set( SOURCE_FILES
    source/Controller.cpp 
    source/EPICSInterface.cpp 
    source/EPICSMagnetInterface.cpp 
    source/LoggingSystem.cpp 
    source/MagnetController.cpp 
    source/MagnetPython.cpp 
    source/main.cpp
)

set( INCLUDE_FILES
    include/Controller.h 
    include/EPICSInterface.h
    include/EPICSMagnetInterface.h 
    include/LoggingSystem.h 
    include/MagnetController.h 
    include/MagnetPython.h 
)

set( TEST_FILES
	ControllerTest.cpp
	EPICSInterfaceTest.cpp
	EPICSMagnetInterfaceTest.cpp
	LoggingSystemTest.cpp
	MagnetControllerTest.cpp
	MagnetPythonTest.cpp
	)

add_library(Magnet ${SOURCE_FILES} ${INCLUDE_FILES})
TARGET_LINK_LIBRARIES(Magnet ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET Magnet PROPERTY FOLDER "Magnet")

include_directories(include)
add_subdirectory(test)