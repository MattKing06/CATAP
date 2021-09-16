message( "Processing Factories Unit Tests")
enable_testing()
add_executable(FactoryTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(FactoryTests PRIVATE ${Boost_INCLUDE_DIR} ${Python3_INCLUDE_DIRS}
												${PROJECT_SOURCE_DIR}/Factories/include ${PROJECT_SOURCE_DIR}/EPICSInterface/include
												${PROJECT_SOURCE_DIR}/PV/include ${PROJECT_SOURCE_DIR}/Hardware/include 
												${PROJECT_SOURCE_DIR}/Utilities/include ${PROJECT_SOURCE_DIR}/ConfigReader/include)
TARGET_LINK_LIBRARIES(FactoryTests PRIVATE ${Boost_LIBRARIES} ${Python3_LIBRARIES}
										    Hardware
											Factories PV
											ConfigReader)
#move testing binaries to separate folder
set_target_properties(FactoryTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET FactoryTests PROPERTY FOLDER "UnitTests")
if ( Boost_FOUND )
	add_definitiOns(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_compOnent(testName ${testFile} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/FactoryTests
			 )
	endforeach(testFile ${TEST_FILES})
	if(WIN32)
		#file(GLOB RELEASE_LIBS ${THIRD_PARTY_LIBS}/Windows/x64/Release/*.dll)
		#file(GLOB DEBUG_LIBS ${THIRD_PARTY_LIBS}/Windows/x64/Debug/*.dll)
		#message("THIRD PARTY LOCATION: " ${THIRD_PARTY_LIBS})
		#message("REL LIBS: " ${RELEASE_LIBS})
		#message("DEB LIBS: " ${DEBUG_LIBS})
		get_target_property(RELEASE_OUTPUT_DIRECTORY FactoryTests RUNTIME_OUTPUT_DIRECTORY)
		get_target_property(DEBUG_OUTPUT_DIRECTORY FactoryTests RUNTIME_OUTPUT_DIRECTORY)
		add_custom_command(TARGET FactoryTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Debug $<1:${DEBUG_OUTPUT_DIRECTORY}/Debug>)
		add_custom_command(TARGET FactoryTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/thirdparty/Windows/x64/Release $<1:${RELEASE_OUTPUT_DIRECTORY}/Release>)
	endif(WIN32)
endif(Boost_FOUND)
