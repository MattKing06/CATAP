message( "Processing PYTHON Interface source files...")
set( SOURCE_FILES
    source/PYTHONInterface.cpp 
    source/MagnetPYTHONInterface.cpp 
)

set( INCLUDE_FILES
    include/PYTHONInterface.h 
    include/MagnetPYTHONInterface.h 
)

set( TEST_FILES
    PYTHONInterfaceTest.cpp
)
cmake_policy(SET CMP0022 NEW)
find_package(PYTHONLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS PYTHON3)
add_library(PYTHONInterface SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(PYTHONInterface PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
#set_target_properties(PYTHONInterface PROPERTIES ENABLE_EXPORTS On)
target_include_directories(PYTHONInterface PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(PYTHONInterface PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(PYTHONInterface PRIVATE include)
target_include_directories(PYTHONInterface PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(PYTHONInterface PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(PYTHONInterface PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(PYTHONInterface PRIVATE ${PROJECT_SOURCE_DIR}/Factories/include)
TARGET_LINK_LIBRARIES(PYTHONInterface PUBLIC
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 PRIVATE
					 Factories
					 Hardware
					 ConfigReader)
set_target_properties(PYTHONInterface PROPERTIES OUTPUT_NAME "CATAP")
set_target_properties(PYTHONInterface PROPERTIES SUFFIX ".pyd")
set_property(TARGET PYTHONInterface PROPERTY FOLDER "PYTHONInterface")

add_subdirectory(test)
