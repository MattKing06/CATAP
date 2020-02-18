message( "Processing Python Interface source files...")
set( SOURCE_FILES
    source/PythonInterface.cpp 
    source/MagnetPythonInterface.cpp 
    source/BPMPythonInterface.cpp 
    source/ChargePythonInterface.cpp 
)

set( INCLUDE_FILES
    include/PythonInterface.h 
    include/MagnetPythonInterface.h 
    include/BPMPythonInterface.h 
    include/ChargePythonInterface.h 
)

set( TEST_FILES
    PythonInterfaceTest.cpp
)
cmake_policy(SET CMP0022 NEW)
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
add_library(PythonInterface SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  message("COMPILING PYTHON INTERFACE WITH -fPIC")
  SET_TARGET_PROPERTIES(PythonInterface PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
#set_target_properties(PythonInterface PROPERTIES ENABLE_EXPORTS on)
target_include_directories(PythonInterface PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(PythonInterface PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(PythonInterface PRIVATE include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(PythonInterface PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(PythonInterface PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(PythonInterface PRIVATE ${PROJECT_SOURCE_DIR}/Factories/include)
TARGET_LINK_LIBRARIES(PythonInterface PUBLIC
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 PRIVATE
					 Factories
					 Hardware
					 ConfigReader
					 Utilities)
set_target_properties(PythonInterface PROPERTIES OUTPUT_NAME "CATAP")
set_target_properties(PythonInterface PROPERTIES SUFFIX ".pyd")
set_property(TARGET PythonInterface PROPERTY FOLDER "PythonInterface")

add_subdirectory(test)
