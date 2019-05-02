message( "Processing HardwareFactory Unit Tests")
enable_testing()
if ( Boost_FOUND )
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		if (NOT TARGET HardwareFactory)
			# if HardwareFactory library hasn't been built yet, build it
			add_library(HardwareFactory)
		endif(NOT TARGET HardwareFactory)
		#Add a compile target
		add_executable(${testName} ${testSource})
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/HardwareFactory/include ${PROJECT_SOURCE_DIR}/Magnet/include ${PROJECT_SOURCE_DIR}/ConfigReader/include ${PROJECT_SOURCE_DIR}/Hardware/include ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
		TARGET_LINK_LIBRARIES(${testName} optimized ${Boost_LIBRARIES} ${PYTHON_LIBRARY} HardwareFactory LoggingSystem Magnet ConfigReader)
		TARGET_LINK_LIBRARIES(${testName} debug ${Boost_LIBRARIES}; ${PYTHON_LIBRARY}; HardwareFactory; LoggingSystem; Magnet; ConfigReader;)
		message("boost libraries: " ${PYTHON_LIBRARY})
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testSource)
	#add_test(HardwareFactoryTest ${TEST_FILES})
	#set_property( TARGET HardwareFactoryTest PROPERTY FOLDER "UnitTests" )
	#target_include_directories(HardwareFactoryTest PRIVATE ${BOOST_INCLUDE_DIRS} )
endif(Boost_FOUND)