message( "Processing Hardware source files...")
set( SOURCE_FILES
    source/Hardware.cpp
    source/Magnet.cpp
    source/BPM.cpp
    source/Charge.cpp
    source/Camera.cpp
    source/Screen.cpp
    source/Valve.cpp
    source/IMG.cpp
    source/LaserEnergyMeter.cpp
    source/LaserHWP.cpp
    source/LaserMirror.cpp
    source/LED.cpp
    source/Lighting.cpp
    source/LinacPID.cpp
    source/LLRF.cpp
    source/RFHeartbeat.cpp
    source/RFModulator.cpp
    source/RFProtection.cpp
    source/Stage.cpp
    source/Shutter.cpp
)

set( INCLUDE_FILES
    include/Hardware.h
    include/Magnet.h
    include/BPM.h
    include/Charge.h
    include/Camera.h
    include/Screen.h
    include/Valve.h
    include/IMG.h
    include/LaserEnergyMeter.h
    include/LaserHWP.h
    include/LaserMirror.h
    include/LED.h
    include/Lighting.h
    include/LinacPID.h
    include/LLRF.h
    include/RFHeartbeat.h
    include/RFModulator.h
    include/RFProtection.h
    include/Stage.h
    include/Shutter.h
)

set( TEST_FILES
	HardwareTestHelper.cpp
	HardwareTest.cpp
	MagnetTest.cpp
	CameraTest.cpp
	BPMTest.cpp
	ChargeTest.cpp
	ScreenTest.cpp
    ValveTest.cpp
    IMGTest.cpp
    LaserEnergyMeterTest.cpp
    LaserHWPTest.cpp
    LaserMirrorTest.cpp
    LEDTest.cpp
    LightingTest.cpp
    LinacPIDTest.cpp
    LLRFTest.cpp
    RFHeartbeatTest.cpp
    RFModulatorTest.cpp
    RFProtectionTest.cpp
    StageTest.cpp
    ShutterTest.cpp
)
cmake_policy(SET CMP0022 NEW)
add_library(Hardware STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
#find_package(PythonLibs ${PY_VERSION})
#find_package(Boost 1.63 REQUIRED COMPONENTS python3)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Hardware PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_include_directories(Hardware PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(Hardware PRIVATE include)
target_include_directories(Hardware PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(Hardware PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Hardware PUBLIC ${Python3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(Hardware
					  PUBLIC
					    PV
                        Threads::Threads
					  PRIVATE
						Utilities
			     	    ${Boost_LIBRARIES}
						${Python3_LIBRARIES}
		    			EPICSInterface
						LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
add_subdirectory(test)
