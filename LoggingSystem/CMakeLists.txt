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
target_include_directories(LoggingSystem PUBLIC include ${Boost_INCLUDE_DIRS})
target_link_libraries(LoggingSystem PUBLIC ${Boost_LIBRARIES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(LoggingSystem PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
set_property(TARGET LoggingSystem PROPERTY FOLDER "LoggingSystem")
set_target_properties(LoggingSystem PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
                      )
INSTALL(FILES ${INCLUDE_FILES} DESTINATION "${CMAKE_BINARY_DIR}/include/LoggingSystem")
add_subdirectory(test)
