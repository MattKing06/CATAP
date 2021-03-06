message( "Processing Factories Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework)
add_executable(FactoryTests ${TEST_FILES})
cmake_policy(SET CMP0023 NEW)
#link to Boost libs AND whatever else
target_include_directories(FactoryTests PRIVATE ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR}
												${PROJECT_SOURCE_DIR}/Factories/include ${PROJECT_SOURCE_DIR}/EPICSInterface/include
												${PROJECT_SOURCE_DIR}/PV/include ${PROJECT_SOURCE_DIR}/Hardware/include 
												${PROJECT_SOURCE_DIR}/Utilities/include ${PROJECT_SOURCE_DIR}/ConfigReader/include)
TARGET_LINK_LIBRARIES(FactoryTests PRIVATE ${Boost_LIBRARIES}
										    Hardware
											Factories PV
											ConfigReader)
#move testing binaries to separate folder
set_target_properties(FactoryTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
set_property(TARGET FactoryTests PROPERTY FOLDER "UnitTests")
if ( Boost_FOUND )
	add_definitiOns(${Boost_LIB_DIAGNOSTIC_DEFINITIOnS})
    foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_compOnent(testName ${testFile} NAME_WE)
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/FactoryTests
			 )
	endforeach(testFile ${TEST_FILES})
endif(Boost_FOUND)
