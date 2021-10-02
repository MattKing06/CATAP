message( "Processing PILaserSystem Unit Tests")
enable_testing()
add_executable(PILaserSystemTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(PILaserSystemTests PRIVATE ${Boost_INCLUDE_DIR}
                                                ${PROJECT_SOURCE_DIR}/HighLevelSystems/PILaserSystem/include
												${PROJECT_SOURCE_DIR}/Factories/include ${PROJECT_SOURCE_DIR}/EPICSInterface/include
												${PROJECT_SOURCE_DIR}/PV/include ${PROJECT_SOURCE_DIR}/Hardware/include 
												${PROJECT_SOURCE_DIR}/Utilities/include ${PROJECT_SOURCE_DIR}/ConfigReader/include)
TARGET_LINK_LIBRARIES(PILaserSystemTests PRIVATE ${Boost_LIBRARIES}
                                            PILaserSystem
										    Hardware
											Factories PV
											ConfigReader)

if ( Boost_FOUND )
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_compOnent(testName ${testFile} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/PILaserSystemTests
			 )
	endforeach(testFile ${TEST_FILES})
endif(Boost_FOUND)
#move testing binaries to separate folder
set_target_properties(PILaserSystemTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET PILaserSystemTests PROPERTY FOLDER "UnitTests")