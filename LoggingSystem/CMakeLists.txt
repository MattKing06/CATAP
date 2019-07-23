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
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(LoggingSystem PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
set_property(TARGET LoggingSystem PROPERTY FOLDER "LoggingSystem")
add_subdirectory(test)
