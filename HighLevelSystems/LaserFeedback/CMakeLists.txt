set(SOURCE_FILES 
    source/LaserFeedback.cpp
)

set(INCLUDE_FILES
    include/LaserFeedback.h
)

set(TEST_FILES
    LaserFeedbackTest.cpp
    LaserFeedbackTestHelper.cpp
)

cmake_policy(SET CMP0022 NEW)
add_library(LaserFeedback STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(LaserFeedback PUBLIC ${Boost_INCLUDE_DIR})
# target_include_directories(${LIBNAME} PUBLIC ${PYTHON_INCLUDE_DIR})
target_include_directories(LaserFeedback PRIVATE include)
# target_include_directories(${LIBNAME} PRIVATE GENERAL_INCLUDE_FILES)
target_include_directories(LaserFeedback PUBLIC ${PROJECT_SOURCE_DIR}/ConfigReader/include)
target_include_directories(LaserFeedback PRIVATE ${PROJECT_SOURCE_DIR}/LoggingSystem/include)
target_include_directories(LaserFeedback PRIVATE ${PROJECT_SOURCE_DIR}/Hardware/include)
# target_include_directories(${LIBNAME} PRIVATE ${PROJECT_SOURCE_DIR}/EPICSTools/include)
target_include_directories(LaserFeedback PRIVATE ${PROJECT_SOURCE_DIR}/Factories/include)
target_include_directories(LaserFeedback PRIVATE ${PROJECT_SOURCE_DIR}/Utilities/include)
TARGET_LINK_LIBRARIES(LaserFeedback PUBLIC
						${Boost_LIBRARIES}
						PRIVATE
						Factories
						Hardware
						ConfigReader
						Utilities)
set_property(TARGET LaserFeedback PROPERTY FOLDER "HighLevelSystems")
add_subdirectory(test)