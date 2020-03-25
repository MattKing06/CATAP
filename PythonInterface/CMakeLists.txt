message( "Processing Python Interface source files...")
set( SOURCE_FILES
    source/DataTypesPythonInterface.cpp
 #   source/PythonInterface.cpp 
    source/ValvePythonInterface.cpp
    source/MagnetPythonInterface.cpp 
    source/BPMPythonInterface.cpp 
    source/ChargePythonInterface.cpp 
    source/ScreenPythonInterface.cpp 
    source/GeneralMonitorPythonInterface.cpp
	source/GlobalStatesPythonInterface.cpp
    source/HardwareFactoryPythonInterface.cpp
    source/IMGPythonInterface.cpp
    source/LaserMirrorPythonInterface.cpp
    source/LEDPythonInterface.cpp
    source/LightingPythonInterface.cpp
    source/RFHeartbeatPythonInterface.cpp
    source/RFModulatorPythonInterface.cpp
    source/RFProtectionPythonInterface.cpp
    source/StagePythonInterface.cpp
    source/ShutterPythonInterface.cpp
)

set( INCLUDE_FILES
    include/DataTypesPythonInterface.h
 #   include/PythonInterface.h 
    include/MagnetPythonInterface.h 
    include/ValvePythonInterface.h
    include/BPMPythonInterface.h 
    include/ChargePythonInterface.h 
    include/ScreenPythonInterface.h 
    include/GeneralMonitorPythonInterface.h
	include/GlobalStatesPythonInterface.h
    include/HardwareFactoryPythonInterface.h
    include/HardwarePythonInterface.h
	include/IMGPythonInterface.h
    include/LaserMirrorPythonInterface.h
    include/LEDPythonInterface.h
    include/LightingPythonInterface.h
    include/RFHeartbeatPythonInterface.h
    include/RFModulatorPythonInterface.h
    include/RFProtectionPythonInterface.h
    include/StagePythonInterface.h
    include/ShutterPythonInterface.h
)

set( TEST_FILES
    PythonInterfaceTest.cpp
)
if(WIN32)
	set(THIRD_PARTY_LIB_DIRS 
		${Boost_LIBRARY_DIRS}/boost_filesystem-vc140-mt-1_63.dll
		${Boost_LIBRARY_DIRS}/boost_python3-vc140-mt-1_63.dll
		${Boost_LIBRARY_DIRS}/boost_python-vc140-mt-1_63.dll
		${Boost_LIBRARY_DIRS}/boost_system-vc140-mt-1_63.dll
		${EPICS_COM_DLL}
		${EPICS_CA_DLL}
		)
endif(WIN32)
if(UNIX)
	set(THIRD_PARTY_LIB_DIRS 
		${Boost_LIBRARY_DIRS}/libboost_filesystem.so
		${Boost_LIBRARY_DIRS}/libboost_python3.so
		${Boost_LIBRARY_DIRS}/libboost_python.so
		${Boost_LIBRARY_DIRS}/libboost_system.so
		${EPICS_COM_LIB}
		${EPICS_CA_LIB}
		)
endif(UNIX)
cmake_policy(SET CMP0022 NEW)
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
foreach(FILE ${SOURCE_FILES})
    # GET ASSOCIATED INCLUDE FILE BASED FROM SOURCE FILE
    string(REPLACE "source/" "include/" INCLUDE_FILENAME "${FILE}")
	string(REPLACE ".cpp" ".h" INCLUDE_FILE "${INCLUDE_FILENAME}")
	message("INCLUDE FILE: ${INCLUDE_FILE} ")
	# GET LIB NAME AND LIB FOLDER NAME FROM SOURCE FILE
    string(REPLACE "PythonInterface.cpp" "" LIBFILENAME "${FILE}")
    message("LIBRARY NAME: ${LIBNAME}")
    string(REGEX REPLACE "source/" "_" LIBNAME "${LIBFILENAME}")
	string(REGEX REPLACE "_" "" LIBFOLDERNAME "${LIBNAME}")
	add_library(${LIBNAME} SHARED ${FILE} ${INCLUDE_FILE})
	IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
	  message("COMPILING ${LIBNAME} WITH -fPIC")
	  SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES COMPILE_FLAGS "-fPIC")
	ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
	#set_target_properties(PythonInterface PROPERTIES ENABLE_EXPORTS on)
	target_include_directories(${LIBNAME} PUBLIC ${Boost_INCLUDE_DIR})
	target_include_directories(${LIBNAME} PUBLIC ${PYTHON_INCLUDE_DIR})
	target_include_directories(${LIBNAME} PRIVATE include)
	target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
	target_include_directories(${LIBNAME} PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
	target_include_directories(${LIBNAME} PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
	target_include_directories(${LIBNAME} PRIVATE ${PROJECT_SOURCE_DIR}/Factories/include)
	TARGET_LINK_LIBRARIES(${LIBNAME} PUBLIC
						 ${Boost_LIBRARIES}
						 ${PYTHON_LIBRARY}
						 PRIVATE
						 Factories
						 Hardware
						 ConfigReader
						 Utilities)
	if (UNIX)
	  set_target_properties(${LIBNAME} PROPERTIES PREFIX "")
	endif(UNIX)
	set_target_properties(${LIBNAME} PROPERTIES OUTPUT_NAME ${LIBNAME})
	set_target_properties(${LIBNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/PythonInterface/CATAP/${LIBFOLDERNAME}>)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/init/__init__.py.in ${CMAKE_BINARY_DIR}/PythonInterface/CATAP/${LIBFOLDERNAME}/__init__.py)
	add_custom_command(TARGET ${LIBNAME} COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/init/__init__.py" "${CMAKE_BINARY_DIR}/PythonInterface/CATAP/")
	if(WIN32)
		set_target_properties(${LIBNAME} PROPERTIES SUFFIX ".pyd")
	endif(WIN32)
	foreach(LIBRARY ${THIRD_PARTY_LIB_DIRS})
		add_custom_command(TARGET ${LIBNAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY}" $<1:${CMAKE_BINARY_DIR}/PythonInterface/CATAP/${LIBFOLDERNAME}> )
		add_custom_command(TARGET ${LIBNAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY}" $<1:${CMAKE_BINARY_DIR}/PythonInterface/CATAP/> )
	endforeach(LIBRARY)
	set_property(TARGET ${LIBNAME} PROPERTY FOLDER "PythonInterface")
	# need to use Boost_LIBRARY_DIRS variable to move over the dlls to each sub-directory of CATAP, if they are not there.
	# may be best to use the target's LIBRARY_OUTPUT_DIRECTORY...

endforeach(FILE)


#add_subdirectory(test)
