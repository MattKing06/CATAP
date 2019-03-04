message( "Processing LoggingSystem source files...")
set( SOURCE_FILES
    source/LoggingSystem.cpp 
)

set( INCLUDE_FILES
    include/LoggingSystem.h 
)

set( TEST_FILES
    LoggingSystemTest.cpp
)
add_library(LoggingSystem ${SOURCE_FILES} ${INCLUDE_FILES})
TARGET_LINK_LIBRARIES(LoggingSystem ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET LoggingSystem PROPERTY FOLDER "LoggingSystem")


add_subdirectory(test)