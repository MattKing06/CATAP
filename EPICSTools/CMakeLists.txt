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
target_include_directories(EPICSTools PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(EPICSTools PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(EPICSTools PUBLIC ${Python3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(EPICSTools
					  PUBLIC
					    PV
					  PRIVATE
						Utilities
			     	    ${Boost_LIBRARIES}
						${Python3_LIBRARY}
		    			EPICSInterface
						LoggingSystem)
set_property(TARGET EPICSTools PROPERTY FOLDER "EPICSTools")
add_subdirectory(test)
