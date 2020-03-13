message( "Processing Hardware Factory source files...")
set( SOURCE_FILES
    source/HardwareFactory.cpp
    source/MagnetFactory.cpp
    source/BPMFactory.cpp
    source/ChargeFactory.cpp
    source/ScreenFactory.cpp
    source/ValveFactory.cpp
    source/GeneralMonitorFactory.cpp
    source/IMGFactory.cpp
    source/LaserMirrorFactory.cpp
    source/LEDFactory.cpp
    source/LightingFactory.cpp
    source/RFHeartbeatFactory.cpp
    source/RFModulatorFactory.cpp
    source/RFProtectionFactory.cpp
    source/StageFactory.cpp
    source/ShutterFactory.cpp
)

set( INCLUDE_FILES
    include/HardwareFactory.h 
    include/MagnetFactory.h
    include/BPMFactory.h
    include/ChargeFactory.h
    include/ScreenFactory.h
    include/ValveFactory.h
    include/GeneralMonitorFactory.h
    include/IMGFactory.h
    include/LaserMirrorFactory.h
    include/LEDFactory.h
    include/LightingFactory.h
    include/RFHeartbeatFactory.h
    include/RFModulatorFactory.h
    include/RFProtectionFactory.h
    include/StageFactory.h
    include/ShutterFactory.h
)

set( TEST_FILES
    FactoryTestHelper.cpp
    HardwareFactoryTest.cpp
    MagnetFactoryTest.cpp
    BPMFactoryTest.cpp
    ChargeFactoryTest.cpp
    ScreenFactoryTest.cpp
    ValveFactoryTest.cpp
    GeneralMonitorFactoryTest.cpp
    IMGFactoryTest.cpp
    LaserMirrorFactoryTest.cpp
    LEDFactoryTest.cpp
    LightingFactoryTest.cpp
    RFHeartbeatFactoryTest.cpp
    RFModulatorFactoryTest.cpp
    RFProtectionFactoryTest.cpp
    StageFactoryTest.cpp
    ShutterFactoryTest.cpp
)

cmake_policy(SET CMP0022 NEW)
find_package(Boost 1.63 REQUIRED COMPONENTS filesystem python3)
add_library(Factories STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Factories PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Factories PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(Factories PRIVATE ${PYTHON_INCLUDE_DIR})
target_include_directories(Factories PRIVATE include)
target_include_directories(Factories PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(Factories PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Factories PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(Factories PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(Factories PUBLIC ${YAML_PARSER_DIR}/include)
if(WIN32)
  TARGET_LINK_LIBRARIES(Factories PUBLIC debug ${YAML_PARSER_LIB_DEBUG_DIR}/libyaml-cppmdd.lib)
  TARGET_LINK_LIBRARIES(Factories PUBLIC optimized ${YAML_PARSER_LIB_RELEASE_DIR}/libyaml-cppmd.lib)
endif(WIN32)
if(UNIX)
  TARGET_LINK_LIBRARIES(Factories PUBLIC general ${YAML_PARSER_LIB_DIR}/libyaml-cpp.so)
endif(UNIX)
TARGET_LINK_LIBRARIES(Factories PUBLIC
					 ${Boost_LIBRARIES}
					 ${PYTHON_LIBRARY}
					 PRIVATE
					 ConfigReader
					 LoggingSystem
					 Hardware)
set_property(TARGET Factories PROPERTY FOLDER "Factories")

add_subdirectory(test)
