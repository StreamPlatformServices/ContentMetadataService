﻿project(ContentMetadataCommon_Tests)

file(GLOB_RECURSE TEST_FILES "*.cpp" "*.hpp")

add_executable(ContentMetadataCommon_Tests ${TEST_FILES})

target_include_directories(ContentMetadataApi_Tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(ContentMetadataCommon_Tests
    ContentMetadataCommon
    gtest
    gtest_main
    gmock
    gmock_main
)

add_test(NAME ContentMetadataCommon_Tests COMMAND ContentMetadataCommon_Tests)

install(TARGETS ContentMetadataCommon_Tests
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataCommon_Tests"
)

if(WIN32)
    install(FILES "$<TARGET_PDB_FILE:ContentMetadataCommon_Tests>"
        DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataCommon_Tests"
        OPTIONAL
    )
endif()