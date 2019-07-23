message( "Processing PV source files...")
set( SOURCE_FILES
    source/PV.cpp
)

set( INCLUDE_FILES
    include/PV.h
)

set( TEST_FILES
	PVTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(PV STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(PV PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
#EPICS RELATED VARS
target_include_directories(PV PUBLIC ${EPICS_DIR}/include)
target_include_directories(PV PUBLIC ${EPICS_OS_INCLUDES})
target_include_directories(PV PUBLIC include)
TARGET_LINK_LIBRARIES(PV PUBLIC ${EPICS_COM_LIB} ${EPICS_CA_LIB})
set_property(TARGET PV PROPERTY FOLDER "PV")
include_directories(include)
#add_subdirectory(test)
