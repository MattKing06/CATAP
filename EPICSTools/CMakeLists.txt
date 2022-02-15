message("Processing EPICSTools source files...")

set( SOURCE_FILES
    source/EPICSTools.cpp
    source/Listener.cpp
    source/Getter.cpp
    source/Putter.cpp
    source/UpdateFunctions.cpp
)

set( INCLUDE_FILES
    include/EPICSTools.h
    include/Listener.h
    include/Getter.h
    include/Putter.h
    include/UpdateFunctions.h
)

set( TEST_FILES
    EPICSToolsTest.cpp
    EPICSToolsTestHelper.cpp
)

cmake_policy(SET CMP0022 NEW)
add_library(EPICSTools STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(EPICSTools PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(EPICSTools PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(EPICSTools PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(EPICSTools PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(EPICSTools PUBLIC ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(EPICSTools PRIVATE include)
target_include_directories(EPICSTools PUBLIC ${Boost_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(EPICSTools
					  PUBLIC
					    PV
					  PRIVATE
						Utilities
			     	    ${Boost_LIBRARIES}
		    			EPICSInterface
						LoggingSystem)
if (BUILD_PYTHON)
TARGET_LINK_LIBRARIES(EPICSTools PUBLIC ${Python3_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(EPICSTools PUBLIC ${Python3_INCLUDE_DIRS})
endif(BUILD_PYTHON)
set_property(TARGET EPICSTools PROPERTY FOLDER "EPICSTools")
set_target_properties(EPICSTools PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
                      )
INSTALL(FILES ${INCLUDE_FILES} DESTINATION "${CMAKE_BINARY_DIR}/include/EPICSTools")
add_subdirectory(test)
