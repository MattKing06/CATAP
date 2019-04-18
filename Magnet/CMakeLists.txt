message( "Processing source files...")
set( SOURCE_FILES
    #source/EPICSInterface.cpp 
    #source/EPICSMagnetInterface.cpp 
    source/Magnet.cpp
    #source/MagnetPython.cpp 
)

set( INCLUDE_FILES
   # include/EPICSInterface.h
    #include/EPICSMagnetInterface.h
    include/Magnet.h 
   # include/MagnetPython.h 
)

set( TEST_FILES
	#EPICSInterfaceTest.cpp
	#EPICSMagnetInterfaceTest.cpp
	MagnetTest.cpp
	#MagnetPythonTest.cpp
	)
cmake_policy(SET CMP0022 NEW)
add_library(Magnet ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(Magnet PUBLIC ${PROJECT_SOURCE_DIR}/Hardware/include)
target_include_directories(Magnet PUBLIC ${YAML_PARSER_DIR}/include)
TARGET_LINK_LIBRARIES(Magnet LINK_PUBLIC ${Boost_LIBRARIES} ${EPICS_LIBRARYDIR}Com.lib ${EPICS_LIBRARYDIR}ca.lib ConfigReader)
set_property(TARGET Magnet PROPERTY FOLDER "Magnet")
include_directories(include)
add_subdirectory(test)