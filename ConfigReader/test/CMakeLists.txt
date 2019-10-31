message( "Processing ConfigReader Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework filesystem)
if ( Boost_FOUND )
		#Extract filename without extension
		get_filename_component(testName ConfigReaderTest.cpp NAME_WE)
		#Add a compile target
		add_executable(${testName} ConfigReaderTest.cpp)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS}
													   ${CMAKE_SOURCE_DIR}/ConfigReader/include
													   ${CMAKE_SOURCE_DIR}/Hardware/include
													   ${CMAKE_SOURCE_DIR}/LoggingSystem/include)
		if(WIN32)
		  TARGET_LINK_LIBRARIES(${testName} PRIVATE ${Boost_LIBRARIES} ConfigReader LoggingSystem Hardware)
		endif(WIN32)
		if(UNIX)
		  TARGET_LINK_LIBRARIES(${testName} INTERFACE general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
		  TARGET_LINK_LIBRARIES(${testName} PRIVATE ${Boost_LIBRARIES} ConfigReader LoggingSystem)
		endif(UNIX)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
endif()
