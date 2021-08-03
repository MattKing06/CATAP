message( "Processing EPICS Interface Unit Tests")
enable_testing()
#find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
#Add a compile target
add_executable(EPICSInterfaceTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(EPICSInterfaceTests PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(EPICSInterfaceTests PRIVATE ${EPICS_DIR}/include)
target_include_directories(EPICSInterfaceTests PRIVATE ${EPICS_OS_INCLUDES})
target_include_directories(EPICSInterfaceTests PRIVATE ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(EPICSInterfaceTests PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(EPICSInterfaceTests PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
target_link_libraries(EPICSInterfaceTests PRIVATE ${Boost_LIBRARIES})
target_link_libraries(EPICSInterfaceTests PRIVATE ${EPICS_COM_LIB} ${EPICS_CA_LIB})
target_link_libraries(EPICSInterfaceTests PRIVATE Utilities)
target_link_libraries(EPICSInterfaceTests PRIVATE EPICSInterface)
target_link_libraries(EPICSInterfaceTests PRIVATE Hardware)
#move testing binaries to separate folder
set_target_properties(EPICSInterfaceTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET EPICSInterfaceTests PROPERTY FOLDER "UnitTests")
message(${CMAKE_CURRENT_BINARY_DIR})
if ( Boost_FOUND )
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/EPICSInterfaceTests
			 )
	endforeach(testSource)
endif()
