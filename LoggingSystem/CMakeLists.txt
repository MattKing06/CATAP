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
target_include_directories(LoggingSystem PUBLIC include ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(LoggingSystem PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
set_property(TARGET LoggingSystem PROPERTY FOLDER "LoggingSystem")


add_subdirectory(test)