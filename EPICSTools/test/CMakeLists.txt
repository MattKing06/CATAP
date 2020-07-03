message( "Processing EPICSTools Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
add_executable(EPICSToolsTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(EPICSToolsTests PRIVATE ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR}
												${PROJECT_SOURCE_DIR}/EPICSTools/include 
												${PROJECT_SOURCE_DIR}/EPICSInterface/include
												${PROJECT_SOURCE_DIR}/LoggingSystem/include
												${PROJECT_SOURCE_DIR}/PV/include
												${PROJECT_SOURCE_DIR}/Utilities/include
                                                ${EPICS_OS_INCLUDES}
                                                ${EPICS_DIR}/include)
target_link_libraries(EPICSToolsTests PRIVATE ${Boost_LIBRARIES})
target_link_libraries(EPICSToolsTests PRIVATE EPICSInterface)
target_link_libraries(EPICSToolsTests PRIVATE Utilities)
target_link_libraries(EPICSToolsTests PRIVATE EPICSTools)
target_link_libraries(EPICSToolsTests PRIVATE LoggingSystem)
#move testing binaries to separate folder
set_target_properties(EPICSToolsTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET EPICSToolsTests PROPERTY FOLDER "UnitTests")
if ( Boost_FOUND )
	add_definitiOns(${Boost_LIB_DIAGNOSTIC_DEFINITIOnS})
    foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_compOnent(testName ${testFile} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/EPICSToolsTests
			 )
	endforeach(testFile ${TEST_FILES})
endif(Boost_FOUND)
