message( "Processing Factories Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
if ( Boost_FOUND )
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testFile} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testFile})
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/Factories/include ${PROJECT_SOURCE_DIR}/EPICSInterface ${PROJECT_SOURCE_DIR}/PV/include ${PROJECT_SOURCE_DIR}/Magnet/include)
		TARGET_LINK_LIBRARIES(${testName} PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${EPICS_COM_LIB} ${EPICS_CA_LIB} Factories PV)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testFile ${TEST_FILES})
endif(Boost_FOUND)