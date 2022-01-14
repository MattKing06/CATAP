set(SOURCE_FILES 
    source/PILaserSystem.cpp
)

set(INCLUDE_FILES
    include/PILaserSystem.h
)

set(TEST_FILES
    PILaserSystemTest.cpp
    PILaserSystemTestHelper.cpp
)

cmake_policy(SET CMP0022 NEW)
add_library(PILaserSystem STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(PILaserSystem PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(PILaserSystem PUBLIC ${Boost_INCLUDE_DIR})
# target_include_directories(${LIBNAME} PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(PILaserSystem PRIVATE include)
# target_include_directories(${LIBNAME} PRIVATE GENERAL_INCLUDE_FILES)
target_include_directories(PILaserSystem PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(PILaserSystem PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(PILaserSystem PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
# target_include_directories(${LIBNAME} PRIVATE ${PROJECT_SOURCE_DIR}/EPICSTools/include)
target_include_directories(PILaserSystem PRIVATE ${PROJECT_SOURCE_DIR}/Factories/include)
target_include_directories(PILaserSystem PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
TARGET_LINK_LIBRARIES(PILaserSystem PUBLIC
						${Boost_LIBRARIES}
						PRIVATE
						Factories
						Hardware
						ConfigReader
						Utilities)
set_property(TARGET PILaserSystem PROPERTY FOLDER "HighLevelSystems")
set_target_properties(PILaserSystem PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
                      )
add_subdirectory(test)