message( "Processing Python Interface Unit Tests")
enable_testing()
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
if ( Boost_FOUND )
	add_definitiOns(${Boost_LIB_DIAGNOSTIC_DEFINITIOnS})
	foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_compOnent(testName ${testFile} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testFile} )
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/PythonInterface/include ${PROJECT_SOURCE_DIR}/Factories/include ${PROJECT_SOURCE_DIR}/Hardware/include ${PROJECT_SOURCE_DIR}/ConfigReader/include ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
		TARGET_LINK_LIBRARIES(${testName} PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} PythonInterface Factories LoggingSystem Hardware ConfigReader)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testFile in ${TEST_FILES})
endif(Boost_FOUND)
