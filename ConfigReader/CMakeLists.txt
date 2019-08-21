message( "Processing ConfigReader source files...")
set( SOURCE_FILES
    source/ConfigReader.cpp 
)

set( INCLUDE_FILES
    include/ConfigReader.h 
)

set( TEST_FILES
    ConfigReaderTest.cpp
)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0079 NEW)
add_library(ConfigReader STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(ConfigReader PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(ConfigReader PUBLIC include)
target_include_directories(ConfigReader PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(ConfigReader PUBLIC ${YAML_PARSER_DIR}/include)
target_include_directories(ConfigReader PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(ConfigReader PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
if(WIN32)
  TARGET_LINK_LIBRARIES(ConfigReader PUBLIC debug ${YAML_PARSER_LIB_DEBUG_DIR}/libyaml-cppmdd.lib)
  TARGET_LINK_LIBRARIES(ConfigReader PUBLIC optimized ${YAML_PARSER_LIB_RELEASE_DIR}/libyaml-cppmd.lib)
endif(WIN32)
if(UNIX)
  TARGET_LINK_LIBRARIES(ConfigReader PUBLIC general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
endif(UNIX)
TARGET_LINK_LIBRARIES(ConfigReader PUBLIC LoggingSystem)
TARGET_LINK_LIBRARIES(ConfigReader PUBLIC Hardware)
set_property(TARGET ConfigReader PROPERTY FOLDER "ConfigReader")
add_subdirectory(test)
