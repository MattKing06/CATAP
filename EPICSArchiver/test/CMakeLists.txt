
enable_testing()
add_executable(EPICSArchiverTests ${TEST_FILES})

target_include_directories(EPICSArchiverTests PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(EPICSArchiverTests PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(EPICSArchiverTests PRIVATE ${PROJECT_SOURCE_DIR}/EPICSArchiver/include)

target_link_libraries(EPICSArchiverTests PRIVATE ${Boost_LIBRARIES})
target_link_libraries(EPICSArchiverTests PRIVATE Utilities)
target_link_libraries(EPICSArchiverTests PRIVATE EPICSArchiver)

set_target_properties(EPICSArchiverTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET EPICSArchiverTests PROPERTY FOLDER "UnitTests")

if ( Boost_FOUND )
	foreach(testSource ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testSource} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/EPICSArchiverTests
			 )
	endforeach(testSource)
	if(WIN32)
		get_target_property(RELEASE_OUTPUT_DIRECTORY EPICSArchiverTests RUNTIME_OUTPUT_DIRECTORY)
		get_target_property(DEBUG_OUTPUT_DIRECTORY EPICSArchiverTests RUNTIME_OUTPUT_DIRECTORY)
		add_custom_command(TARGET EPICSArchiverTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Debug $<1:${DEBUG_OUTPUT_DIRECTORY}/Debug>)
		add_custom_command(TARGET EPICSArchiverTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Release $<1:${RELEASE_OUTPUT_DIRECTORY}/Release>)
	endif(WIN32)
endif()