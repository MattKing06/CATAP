message("Processing Utilities source files...")

set( SOURCE_FILES
    source/PythonTypeConversions.cpp
    source/GlobalStateEnums.cpp
    source/GlobalTypeEnums.cpp
    source/GlobalFunctions.cpp
    source/MagnetPVRecords.cpp
    source/ValvePVRecords.cpp
    source/BPMPVRecords.cpp
    source/CameraPVRecords.cpp
    source/ChargePVRecords.cpp
    source/ScreenPVRecords.cpp
    source/HardwareState.cpp
    source/IMGPVRecords.cpp
    source/LaserEnergyMeterPVRecords.cpp
    source/LaserHWPPVRecords.cpp
    source/LaserMirrorPVRecords.cpp
    source/LEDPVRecords.cpp
    source/LightingPVRecords.cpp
    source/LLRFPVRecords.cpp
    source/RFHeartbeatPVRecords.cpp
    source/RFModulatorPVRecords.cpp
    source/RFProtectionPVRecords.cpp
    source/StagePVRecords.cpp
    source/ShutterPVRecords.cpp
    source/RunningStats.cpp
)
set( INCLUDE_FILES
     include/PythonTypeConversions.h
     include/GlobalStateEnums.h
     include/GlobalTypeEnums.h
     include/GlobalFunctions.h
     include/GlobalConstants.h
     include/HardwareState.h
     include/MagnetPVRecords.h
     include/ValvePVRecords.h
     include/BPMPVRecords.h
     include/CameraPVRecords.h
     include/ChargePVRecords.h
     include/ScreenPVRecords.h
     include/IMGPVRecords.h
     include/LaserEnergyMeterPVRecords.h
     include/LaserHWPPVRecords.h
     include/LaserMirrorPVRecords.h
     include/LEDPVRecords.h
     include/LightingPVRecords.h
     include/LLRFPVRecords.h
     include/RFHeartbeatPVRecords.h
     include/RFModulatorPVRecords.h
     include/RFProtectionPVRecords.h
     include/StagePVRecords.h
     include/ShutterPVRecords.h
     include/RunningStats.h
)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0079 NEW)
find_package(PythonLibs ${PY_VERSION})
find_package(Boost 1.76 REQUIRED COMPONENTS python38)
add_library(Utilities ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  message("COMPILING PYTHON INTERFACE WITH -fPIC")
  SET_TARGET_PROPERTIES(Utilities PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Utilities PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Utilities PRIVATE ${PYTHON_INCLUDE_DIR})
target_include_directories(Utilities PUBLIC ${YAML_PARSER_DIR}/include)
target_include_directories(Utilities PUBLIC include)
if(WIN32)
  TARGET_LINK_LIBRARIES(Utilities PUBLIC debug ${YAML_PARSER_LIB_DEBUG_DIR}/libyaml-cppmdd.lib)
  TARGET_LINK_LIBRARIES(Utilities PUBLIC optimized ${YAML_PARSER_LIB_RELEASE_DIR}/libyaml-cppmd.lib)
endif(WIN32)
if(UNIX)
  TARGET_LINK_LIBRARIES(Utilities PUBLIC general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
endif(UNIX)
TARGET_LINK_LIBRARIES(Utilities
					 PRIVATE
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 )
set_property(TARGET Utilities PROPERTY FOLDER "Utilities")
