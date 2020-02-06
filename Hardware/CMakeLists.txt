message( "Processing Hardware source files...")
set( SOURCE_FILES
    source/Hardware.cpp
    source/Magnet.cpp
    source/BPM.cpp
    source/Charge.cpp
    source/Valve.cpp
)

set( INCLUDE_FILES
    include/Hardware.h
    include/Magnet.h
    include/BPM.h
    include/Charge.h
    include/Valve.h
)

set( TEST_FILES
	HardwareTestHelper.cpp
	HardwareTest.cpp
	MagnetTest.cpp
	BPMTest.cpp
	ChargeTest.cpp
        ValveTest.cpp
)
cmake_policy(SET CMP0022 NEW)
add_library(Hardware STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Hardware PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Hardware PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(Hardware PRIVATE include)
target_include_directories(Hardware PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(Hardware PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Hardware PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(Hardware
					  PUBLIC
					    PV
					  PRIVATE
						Utilities
			     	    ${Boost_LIBRARIES}
						${PYTHON_LIBRARY}
		    			EPICSInterface
						LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
add_subdirectory(test)
