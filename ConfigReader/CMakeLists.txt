message( "Processing source files...")
set( SOURCE_FILES
    source/ConfigReader.cpp 
)

set( INCLUDE_FILES
    include/ConfigReader.h 
)

set( TEST_FILES
    ConfigReaderTest.cpp
)
include_directories(include)

add_library(ConfigReader ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(ConfigReader PUBLIC ${PROJECT_SOURCE_DIR}/Controller/include)
TARGET_LINK_LIBRARIES(ConfigReader ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}/Com.lib ${EPICS_LIBRARYDIR}/ca.lib)
set_property(TARGET ConfigReader PROPERTY FOLDER "ConfigReader")

add_subdirectory(test)