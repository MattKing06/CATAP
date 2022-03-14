message( "Processing LoggingSystem Unit Tests")
enable_testing()
add_definitiOns(-DBOOST_ALL_DYN_LINK)
#find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
if ( Boost_FOUND )
		#Extract filename without extension
	        get_filename_compOnent(testName LoggingSystemTest.cpp NAME_WE)
		#Add a compile target
		add_executable(${testName} LoggingSystemTest.cpp)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS} ${TARGET_SOURCE_DIR}/LoggingSystem/include)
        target_link_libraries(${testName} PRIVATE LoggingSystem)
		target_link_libraries(${testName} PRIVATE ${Boost_LIBRARIES})
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
		if(WIN32)
		#file(GLOB RELEASE_LIBS ${THIRD_PARTY_LIBS}/Windows/x64/Release/*.dll)
		#file(GLOB DEBUG_LIBS ${THIRD_PARTY_LIBS}/Windows/x64/Debug/*.dll)
		#message("THIRD PARTY LOCATION: " ${THIRD_PARTY_LIBS})
		#message("REL LIBS: " ${RELEASE_LIBS})
		#message("DEB LIBS: " ${DEBUG_LIBS})
			get_target_property(RELEASE_OUTPUT_DIRECTORY ${testName} RUNTIME_OUTPUT_DIRECTORY)
			get_target_property(DEBUG_OUTPUT_DIRECTORY ${testName} RUNTIME_OUTPUT_DIRECTORY)
			add_custom_command(TARGET ${testName} COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Debug $<1:${DEBUG_OUTPUT_DIRECTORY}/Debug>)
			add_custom_command(TARGET ${testName} COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Release $<1:${RELEASE_OUTPUT_DIRECTORY}/Release>)
		endif(WIN32)
endif()
