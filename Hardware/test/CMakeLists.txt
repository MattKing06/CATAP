message( "Processing Hardware Unit Tests")
enable_testing()
#find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
#Add a compile target
add_executable(HardwareTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(HardwareTests PRIVATE ${Boost_INCLUDE_DIRS}
                                                 ${YAML_PARSER_DIR}/include
	                                             ${CMAKE_SOURCE_DIR}/Hardware/include
												 ${CMAKE_SOURCE_DIR}/LoggingSystem/include)
target_link_libraries(HardwareTests PUBLIC ${Boost_LIBRARIES})
target_link_libraries(HardwareTests PUBLIC Hardware)
#move testing binaries to separate folder
set_target_properties(HardwareTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET HardwareTests PROPERTY FOLDER "UnitTests")
message(${CMAKE_CURRENT_BINARY_DIR})
if ( Boost_FOUND )
	foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testFile} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/HardwareTests
			 )
	endforeach(testFile)
endif()
