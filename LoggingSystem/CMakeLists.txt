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
add_library(LoggingSystem STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(LoggingSystem PUBLIC include)
set_property(TARGET LoggingSystem PROPERTY FOLDER "LoggingSystem")
add_subdirectory(test)
