message( "Processing Python Interface source files...")
set( SOURCE_FILES
    source/PythonInterface.cpp 
)

set( INCLUDE_FILES
    include/PythonInterface.h 
)

set( TEST_FILES
    PythonInterfaceTest.cpp
)
cmake_policy(SET CMP0022 NEW)
find_package(PythonLibs ${PY_VERSION} REQUIRED python36 python36_d)
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
add_library(PythonInterface SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
set_property(TARGET PythonInterface PROPERTY POSITION_INDEPENDENT_CODE 
ON)
#set_target_properties(PythonInterface PROPERTIES ENABLE_EXPORTS on)
target_include_directories(PythonInterface PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(PythonInterface PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(PythonInterface PUBLIC include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/Factories/include)
target_include_directories(PythonInterface PUBLIC ${PROJECT_SOURCE_DIR}/Magnet/include)
TARGET_LINK_LIBRARIES(PythonInterface PUBLIC
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 Factories)
set_property(TARGET PythonInterface PROPERTY FOLDER "PythonInterface")

add_subdirectory(test)
