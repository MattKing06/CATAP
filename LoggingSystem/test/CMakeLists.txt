message( "Processing LoggingSystem Unit Tests")
enable_testing()
add_definitions(-DBOOST_ALL_DYN_LINK)
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
if ( Boost_FOUND )
		#Extract filename without extension
	get_filename_component(testName LoggingSystemTest.cpp NAME_WE)
		#Add a compile target
		add_executable(${testName} LoggingSystemTest.cpp)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS} ${TARGET_SOURCE_DIR}/LoggingSystem/include)
		target_link_libraries(${testName} PRIVATE ${Boost_LIBRARIES})
		target_link_libraries(${testName} PRIVATE LoggingSystem)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
endif()
