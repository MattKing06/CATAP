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
find_package(Boost 1.63)
cmake_policy(SET CMP0022 NEW)
add_library(ConfigReader SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(ConfigReader PUBLIC include)
target_include_directories(ConfigReader PUBLIC ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
target_include_directories(ConfigReader PUBLIC ${YAML_PARSER_DIR}/include)
target_include_directories(ConfigReader PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
#TARGET_LINK_LIBRARIES(ConfigReader PUBLIC debug ${YAML_PARSER_LIB_DEBUG_DIR}/libyaml-cpp.so) 
TARGET_LINK_LIBRARIES(ConfigReader PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(ConfigReader PUBLIC general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
set_property(TARGET ConfigReader PROPERTY FOLDER "ConfigReader")

add_subdirectory(test)
