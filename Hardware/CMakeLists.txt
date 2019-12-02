message( "Processing Hardware source files...")
set( SOURCE_FILES
    source/Hardware.cpp
    source/Magnet.cpp
    source/BPM.cpp
    source/Charge.cpp
)

set( INCLUDE_FILES
    include/Hardware.h
    include/Magnet.h
    include/BPM.h
    include/Charge.h
)

set( TEST_FILES
	HardwareTest.cpp
	MagnetTest.cpp
	BPMTest.cpp
	ChargeTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Hardware STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(Hardware PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
target_include_directories(Hardware PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/EPICSInterface/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/PV/include)
target_include_directories(Hardware PUBLIC ${PROJECT_SOURCE_DIR}/Utilities/include)
target_include_directories(Hardware PRIVATE include)
target_include_directories(Hardware PRIVATE ${Boost_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(Hardware
					  PUBLIC
					    PV
					  PRIVATE
			     	    ${Boost_LIBRARIES}
		    			EPICSInterface
						LoggingSystem)
set_property(TARGET Hardware PROPERTY FOLDER "Hardware")
add_subdirectory(test)
