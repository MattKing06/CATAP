message( "Processing source files...")
set( SOURCE_FILES
    source/ConfigReader.cpp
    source/Controller.cpp
    source/EPICSInterface.cpp 
    source/EPICSShutterInterface.cpp 
    source/LoggingSystem.cpp
    source/ShutterController.cpp 
    source/ShutterPython.cpp 
    source/main.cpp
)

set( INCLUDE_FILES
    include/ConfigReader.h
    include/Controller.h
    include/EPICSInterface.h
    include/EPICSShutterInterface.h
    include/LoggingSystem.h 
    include/ShutterController.h 
    include/ShutterPython.h 
)

set( TEST_FILES
        ConfigReaderTest.cpp
        ControllerTest.cpp
	EPICSInterfaceTest.cpp
	EPICSShutterInterfaceTest.cpp
        LoggingSystemTest.cpp
	ShutterControllerTest.cpp
	ShutterPythonTest.cpp
	)

add_library(Shutter ${SOURCE_FILES} ${INCLUDE_FILES})
TARGET_LINK_LIBRARIES(Shutter ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET Shutter PROPERTY FOLDER "Shutter")

include_directories(include)
add_subdirectory(test)