message("Processing Utilities source files...")

set(SOURCE_FILES
    source/PythonTypeConversions.cpp
    source/GlobalStateEnums.cpp
    source/GlobalFunctions.cpp
)
set( INCLUDE_FILES
     include/PythonTypeConversions.h
     include/GlobalStateEnums.h
     include/GlobalFunctions.h
     include/GlobalConstants.h
     include/GlobalConstants.h
     include/GlobalConstants.h
)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0079 NEW)
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
add_library(Utilities ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Utilities PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Utilities PRIVATE ${PYTHON_INCLUDE_DIR})
target_include_directories(Utilities PUBLIC include)
TARGET_LINK_LIBRARIES(Utilities
					 PRIVATE
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 )
set_property(TARGET Utilities PROPERTY FOLDER "Utilities")