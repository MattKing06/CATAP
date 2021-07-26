message( "Processing ConfigReader Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework filesystem system)
if(WIN32)
	file(GLOB RELEASE_LIBS {THIRD_PARTY_LIBS}/Windows/x64/Release/*.dll)
	file(GLOB DEBUG_LIBS {THIRD_PARTY_LIBS}/Windows/x64/Debug/*.dll)
	message("REL LIBS: " ${RELEASE_LIBS})
	message("DEB LIBS: " ${DEBUG_LIBS})
endif(WIN32)

if ( Boost_FOUND )
		#Extract filename without extension
		get_filename_compOnent(testName ConfigReaderTest.cpp NAME_WE)
		#Add a compile target
		add_executable(${testName} ConfigReaderTest.cpp)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS}
													   ${CMAKE_SOURCE_DIR}/ConfigReader/include
													   ${CMAKE_SOURCE_DIR}/Hardware/include
													   ${CMAKE_SOURCE_DIR}/Utilities/include
													   ${CMAKE_SOURCE_DIR}/LoggingSystem/include)
		if(WIN32)
		  TARGET_LINK_LIBRARIES(${testName} PRIVATE ${Boost_LIBRARIES} ConfigReader LoggingSystem Utilities Hardware)
		endif(WIN32)
		if(UNIX)
		  TARGET_LINK_LIBRARIES(${testName} INTERFACE general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
		  TARGET_LINK_LIBRARIES(${testName} PRIVATE ${Boost_LIBRARIES} Hardware ConfigReader LoggingSystem Utilities)
		endif(UNIX)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
		if(WIN32)
			get_target_property(RELEASE_OUTPUT_DIRECTORY ${testName} RUNTIME_OUTPUT_DIRECTORY)
			get_target_property(DEBUG_OUTPUT_DIRECTORY ${testName} RUNTIME_OUTPUT_DIRECTORY)
			message("RELEASE BUILD")
			add_custom_command(TARGET ${testName} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Debug $<1:${DEBUG_OUTPUT_DIRECTORY}/Debug>)
			add_custom_command(TARGET ${testName} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Release $<1:${RELEASE_OUTPUT_DIRECTORY}/Release>)
		endif(WIN32)
endif()
