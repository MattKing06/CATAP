message("Processing Utilities source files...")

set(SOURCE_FILES
    source/PythonTypeConversions.cpp
    source/GlobalStateEnums.cpp
    source/GlobalFunctions.cpp
    source/MagnetPVRecords.cpp
    source/ValvePVRecords.cpp
    source/BPMPVRecords.cpp
    source/ChargePVRecords.cpp
    source/ScreenPVRecords.cpp
    source/IMGPVRecords.cpp
    source/LaserMirrorPVRecords.cpp
    source/LEDPVRecords.cpp
    source/LightingPVRecords.cpp
    source/RFHeartbeatPVRecords.cpp
    source/RFModulatorPVRecords.cpp
    source/RFProtectionPVRecords.cpp
    source/StagePVRecords.cpp
    source/ShutterPVRecords.cpp
)
set( INCLUDE_FILES
     include/PythonTypeConversions.h
     include/GlobalStateEnums.h
     include/GlobalFunctions.h
     include/GlobalConstants.h
     include/MagnetPVRecords.h
     include/ValvePVRecords.h
     include/BPMPVRecords.h
     include/ChargePVRecords.h
     include/ScreenPVRecords.h
     include/IMGPVRecords.h
     include/LaserMirrorPVRecords.h
     include/LEDPVRecords.h
     include/LightingPVRecords.h
     include/RFHeartbeatPVRecords.h
     include/RFModulatorPVRecords.h
     include/RFProtectionPVRecords.h
     include/StagePVRecords.h
     include/ShutterPVRecords.h
)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0079 NEW)
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.63 REQUIRED COMPONENTS python3)
add_library(Utilities ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  message("COMPILING PYTHON INTERFACE WITH -fPIC")
  SET_TARGET_PROPERTIES(Utilities PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Utilities PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Utilities PRIVATE ${PYTHON_INCLUDE_DIR})
target_include_directories(Utilities PUBLIC include)
TARGET_LINK_LIBRARIES(Utilities
					 PRIVATE
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 )
set_property(TARGET Utilities PROPERTY FOLDER "Utilities")
