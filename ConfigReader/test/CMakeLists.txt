message( "Processing ConfigReader Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
if ( Boost_FOUND )
		#Extract filename without extension
		get_filename_component(testName ConfigReaderTest.cpp NAME_WE)
		#Add a compile target
		add_executable(${testName} ConfigReaderTest.cpp)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/ConfigReader/include)
		target_link_libraries(${testName} PRIVATE ConfigReader ${Boost_LIBRARIES})
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName}
			 )
endif()