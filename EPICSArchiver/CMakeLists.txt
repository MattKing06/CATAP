message("Processing EPICS Archiver source files ... ")
set(SOURCE_FILES
    source/EPICSArchiverHandler.cpp
)

set(INCLUDE_FILES
    include/EPICSArchiverHandler.h
)

set(TEST_FILES
   test/EPICSArchiverHandlerTest.cpp
)

cmake_policy(SET CMP0022 NEW)
add_library(EPICSArchiver STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(EPICSArchiver PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(EPICSArchiver INTERFACE ${PROJECT_SOURCE_DIR}/LoggingSystem/include
   					 PUBLIC ${BOOST_INCLUDE_DIR} ${Python3_INCLUDE_DIRS}
						include ${PROJECT_SOURCE_DIR}/Utilities/include)

TARGET_LINK_LIBRARIES(EPICSArchiver PUBLIC ${Boost_LIBRARIES}
				           ${Python3_LIBRARIES}
				    PRIVATE
					   LoggingSystem)

SET_PROPERTY(TARGET EPICSArchiver PROPERTY FOLDER "EPICSArchiver")
#add_subdirectory(test)