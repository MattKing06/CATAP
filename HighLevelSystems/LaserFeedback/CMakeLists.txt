set(SOURCE_FILES 
    source/LaserFeedback.cpp
)

set(INCLUDE_FILES
    include/LaserFeedback.h
)

set(TEST_FILES
    LaserFeedbackTest.cpp
)

cmake_policy(SET CMP0022 NEW)
add_library(LaserFeedback STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

set_property(TARGET LaserFeedback PROPERTY FOLDER "HighLevelSystems")