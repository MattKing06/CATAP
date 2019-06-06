message( "Processing EPICS Interface Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
if ( Boost_FOUND )
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testSource})
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS})
		target_include_directories(${testName} PUBLIC ${EPICS_DIR}/include)
		target_include_directories(${testName} PUBLIC ${EPICS_OS_INCLUDES})
		target_include_directories(${testName} PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
		target_include_directories(${testName} PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
		target_include_directories(${testName} PUBLIC ${PROJECT_SOURCE_DIR}/HardwareFactory/include)
		target_link_libraries(${testName} PUBLIC ${Boost_LIBRARIES})
		target_link_libraries(${testName} PUBLIC ${EPICS_COM_LIB} ${EPICS_CA_LIB})
		target_link_libraries(${testName} PUBLIC Hardware)
		target_link_libraries(${testName} PUBLIC Magnet)
		target_link_libraries(${testName} PUBLIC HardwareFactory)
		target_link_libraries(${testName} PUBLIC EPICSInterface)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		message(${CMAKE_CURRENT_BINARY_DIR})
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
endif()
