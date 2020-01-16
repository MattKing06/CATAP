message( "Processing EPICS Interface source files...")
set( SOURCE_FILES
    source/EPICSInterface.cpp
    source/EPICSMagnetInterface.cpp
    source/EPICSBPMInterface.cpp
    source/EPICSChargeInterface.cpp
)

set( INCLUDE_FILES
    include/EPICSInterface.h
    include/EPICSMagnetInterface.h 
    include/EPICSBPMInterface.h 
    include/EPICSChargeInterface.h 
)

set( TEST_FILES
	EPICSInterfaceTestHelper.cpp
    EPICSInterfaceTest.cpp
    EPICSMagnetInterfaceTest.cpp
    EPICSBPMInterfaceTest.cpp
    EPICSChargeInterfaceTest.cpp
)
cmake_policy(SET CMP0022 NEW)
add_library(EPICSInterface STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(EPICSInterface PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
#target_include_directories(EPICSInterface PRIVATE ${EPICS_DIR}/include ${EPICS_OS_INCLUDES})
target_include_directories(EPICSInterface INTERFACE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(EPICSInterface PRIVATE ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(EPICSInterface PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(EPICSInterface PRIVATE ${Boost_INCLUDE_DIR})
#Utilities is currently a header-only library so no need to link it, just include it.
target_include_directories(EPICSInterface PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(EPICSInterface PUBLIC include)
TARGET_LINK_LIBRARIES(EPICSInterface PRIVATE ${EPICS_COM_LIB} ${EPICS_CA_LIB} PV LoggingSystem Hardware)
set_property(TARGET EPICSInterface PROPERTY FOLDER "EPICSInterface")
add_subdirectory(test)
