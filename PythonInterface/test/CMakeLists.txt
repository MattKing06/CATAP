message( "Processing Python Interface Unit Tests")
enable_testing()
find_package(Boost 1.63 REQUIRED COMPONENTS unit_test_framework python3)
if ( Boost_FOUND )
    if(WIN32)
      message(STATUS "WIN32 BUILD: TRUE")
      set(Boost_USE_STATIC_LIBS Off)
      set(Boost_USE_SHARED_LIBS On)
      set(Boost_USE_MULTITHREAD On)
      set(Boost_USE_STATIC_RUNTIME Off)
	  add_definitions(-DBOOST_ALL_DYN_LINK)
    endif(WIN32)
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
	foreach(testFile ${TEST_FILES})
		#Extract filename without extension
		get_filename_component(testName ${testFile} NAME_WE)
		#Add a compile target
		add_executable(${testName} ${testFile} )
		cmake_policy(SET CMP0023 NEW)
		#link to Boost libs AND whatever else
		target_include_directories(${testName} PUBLIC ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/PythonInterface/include ${PROJECT_SOURCE_DIR}/HardwareFactory/include ${PROJECT_SOURCE_DIR}/Magnet/include ${PROJECT_SOURCE_DIR}/ConfigReader/include ${PROJECT_SOURCE_DIR}/Hardware/include ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
		TARGET_LINK_LIBRARIES(${testName} PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} PythonInterface HardwareFactory LoggingSystem Magnet ConfigReader)
		#move testing binaries to separate folder
		set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin)
		set_property(TARGET ${testName} PROPERTY FOLDER "UnitTests")
		#now add the test
		add_test(NAME ${testName}
			 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
			 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName}
			 )
	endforeach(testFile in ${TEST_FILES})
endif(Boost_FOUND)