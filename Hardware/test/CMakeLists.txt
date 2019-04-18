message( "Processing Hardware Unit Tests")
enable_testing()
if ( Boost_FOUND )
	set(Boost_USE_STATIC_LIBS OFF)
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testSource})
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS})
		target_link_libraries(${testName} PUBLIC ${Boost_LIBRARIES})
		target_link_libraries(${testName} PUBLIC Hardware)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		message(${CMAKE_CURRENT_BINARY_DIR})
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
	#add_test(HardwareTest ${TEST_FILES})
	#set_property( TARGET HardwareTest PROPERTY FOLDER "UnitTests" )
	#target_include_directories(HardwareTest PRIVATE ${BOOST_INCLUDE_DIRS} )
endif()