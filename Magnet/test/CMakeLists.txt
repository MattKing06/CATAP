message( "Processing Unit Tests")
enable_testing()
if ( Boost_FOUND )
	set(Boost_USE_STATIC_LIBS ON)
	include_directories(SYSTEM ../Magnet/include)
	include_directories(${Boost_INCLUDE_DIRS})
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testSource})
		#link to Boost libs AND whatever else
		target_link_libraries(${testName} ${Boost_LIBRARIES})
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS})
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
			 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
	#add_test(MagnetTest ${TEST_FILES})
	#set_property( TARGET MagnetTest PROPERTY FOLDER "UnitTests" )
	#target_include_directories(MagnetTest PRIVATE ${BOOST_INCLUDE_DIRS} )
endif()