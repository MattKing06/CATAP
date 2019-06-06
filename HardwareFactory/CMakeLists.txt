message( "Processing Hardware Factory source files...")
set( SOURCE_FILES
    source/HardwareFactory.cpp
    source/MagnetFactory.cpp
)

set( INCLUDE_FILES
    include/HardwareFactory.h 
    include/MagnetFactory.h
)

set( TEST_FILES
    HardwareFactoryTest.cpp
    MagnetFactoryTest.cpp
)
cmake_policy(SET CMP0022 NEW)
find_package(Boost 1.63 REQUIRED COMPONENTS python3 filesystem)
add_library(HardwareFactory SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(HardwareFactory PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(HardwareFactory PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(HardwareFactory PUBLIC include)
target_include_directories(HardwareFactory PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(HardwareFactory PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(HardwareFactory PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(HardwareFactory PUBLIC ${PROJECT_SOURCE_DIR}/Magnet/include)
TARGET_LINK_LIBRARIES(HardwareFactory PUBLIC
					 ${Boost_LIBRARIES}
					 ${Python_LIBRARY}
					 ConfigReader
					 LoggingSystem
					 Hardware
					 Magnet)
set_property(TARGET HardwareFactory PROPERTY FOLDER "HardwareFactory")

add_subdirectory(test)
