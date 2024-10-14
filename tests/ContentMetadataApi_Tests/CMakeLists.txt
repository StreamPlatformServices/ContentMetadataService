project(ContentMetadataApi_Tests)

file(GLOB_RECURSE TEST_FILES "*.cpp" "*.hpp")

add_executable(ContentMetadataApi_Tests ${TEST_FILES})

target_include_directories(ContentMetadataApi_Tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(ContentMetadataApi_Tests
    ContentMetadataApi
    ContentMetadataCore
    ContentMetadataCommon
    gtest
    gtest_main
    gmock
    gmock_main
)

add_test(NAME ContentMetadataApi_Tests COMMAND ContentMetadataApi_Tests)

install(TARGETS ContentMetadataApi
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataApi_Tests"
)

install(FILES "$<TARGET_PDB_FILE:ContentMetadataApi>"
    DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataApi_Tests"
    OPTIONAL
)



