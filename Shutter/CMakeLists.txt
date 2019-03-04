message( "Processing source files...")
set( SOURCE_FILES
    source/EPICSInterface.cpp 
    source/EPICSShutterInterface.cpp 
    source/ShutterController.cpp 
    source/ShutterPython.cpp 
    source/main.cpp
)

set( INCLUDE_FILES
    include/EPICSInterface.h
    include/EPICSShutterInterface.h
    include/ShutterController.h 
    include/ShutterPython.h 
)

set( TEST_FILES
	EPICSInterfaceTest.cpp
	EPICSShutterInterfaceTest.cpp
	ShutterControllerTest.cpp
	ShutterPythonTest.cpp
	)

add_library(Shutter ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Shutter PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Shutter PUBLIC ${PROJECT_SOURCE_DIR}/Controller/include)
TARGET_LINK_LIBRARIES(Shutter ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET Shutter PROPERTY FOLDER "Shutter")

include_directories(include)
add_subdirectory(test)