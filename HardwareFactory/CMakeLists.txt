message( "Processing Hardware Factory source files...")
set( SOURCE_FILES
    source/HardwareFactory.cpp 
)

set( INCLUDE_FILES
    include/HardwareFactory.h 
)

set( TEST_FILES
    HardwareFactoryTest.cpp
)
include_directories(include)

add_library(HardwareFactory ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(HardwareFactory PUBLIC ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/LoggingSystem/include ${PROJECT_SOURCE_DIR}/Hardware/include ${PROJECT_SOURCE_DIR}/Magnet/include)
TARGET_LINK_LIBRARIES(HardwareFactory optimized ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(HardwareFactory ${PYTHON_LIBRARY})
TARGET_LINK_LIBRARIES(HardwareFactory LoggingSystem)
TARGET_LINK_LIBRARIES(HardwareFactory Hardware)
TARGET_LINK_LIBRARIES(HardwareFactory Magnet)
TARGET_LINK_LIBRARIES(HardwareFactory debug ${Boost_debug_LIBRARY} ${PYTHON_DEBUG_LIBRARY} LoggingSystem Hardware Magnet)
set_property(TARGET HardwareFactory PROPERTY FOLDER "HardwareFactory")

add_subdirectory(test)