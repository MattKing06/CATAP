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
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS filesystem python3)
add_library(Factories STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Factories PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Factories PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(Factories PUBLIC include)
target_include_directories(Factories PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(Factories PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Factories PUBLIC ${PROJECT_SOURCE_DIR}/Magnet/include)
TARGET_LINK_LIBRARIES(Factories PUBLIC
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 ConfigReader
					 LoggingSystem
					 Magnet)
TARGET_LINK_LIBRARIES(Factories PRIVATE Magnet)
set_property(TARGET Factories PROPERTY FOLDER "Factories")

add_subdirectory(test)
