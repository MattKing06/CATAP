message( "Processing Unit Tests")
enable_testing()
if ( Boost_FOUND )
	set(Boost_USE_STATIC_LIBS ON)
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		if (NOT TARGET Magnet )
			# if Magnet library hasn't been built yet, build it
			add_library(Magnet ../source/main.cpp)
		endif()
		#Add a compile target
		add_executable(${testName} ${testSource})
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS} PUBLIC ../Magnet/include)
		target_link_libraries(${testName} ${Boost_LIBRARIES})
		target_link_libraries(${testName} Magnet)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
	#add_test(MagnetTest ${TEST_FILES})
	#set_property( TARGET MagnetTest PROPERTY FOLDER "UnitTests" )
	#target_include_directories(MagnetTest PRIVATE ${BOOST_INCLUDE_DIRS} )
endif()