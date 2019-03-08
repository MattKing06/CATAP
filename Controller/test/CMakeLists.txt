message( "Processing Unit Tests")
enable_testing()
if ( Boost_FOUND )
	set(Boost_USE_STATIC_LIBS ON)
	include_directories(../Controller/include)
	include_directories(${PROJECT_SOURCE_DIR}/LoggingSystem/include)
	include_directories(${Boost_INCLUDE_DIRS})
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		if (NOT TARGET Controller )
			# if Shutter library hasn't been built yet, build it
			add_library(Controller ../source/main.cpp)
		endif()
		#Add a compile target
		add_executable(${testName} ${testSource})
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS})
		target_include_directories(${testName} PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
		target_link_libraries(${testName} ${Boost_LIBRARIES})
		target_link_libraries(${testName} Controller)
        target_link_libraries(${testName} LoggingSystem)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
endif()