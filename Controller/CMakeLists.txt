message( "Processing source files...")
set( SOURCE_FILES
    source/Controller.cpp 
)

set( INCLUDE_FILES
    include/Controller.h 
)

set( TEST_FILES
    ControllerTest.cpp
)
include_directories(include)

add_library(Controller ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Controller PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
TARGET_LINK_LIBRARIES(Controller ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET Controller PROPERTY FOLDER "Controller")

add_subdirectory(test)