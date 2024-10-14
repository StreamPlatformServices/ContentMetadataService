project(ContentMetadataRepository_Tests)

file(GLOB_RECURSE TEST_FILES "*.cpp" "*.hpp")

add_executable(ContentMetadataRepository_Tests ${TEST_FILES})

target_include_directories(ContentMetadataRepository_Tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external/libpqxx/include"
    "${CMAKE_BINARY_DIR}/external/libpqxx/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(ContentMetadataRepository_Tests
    ContentMetadataRepository
    ContentMetadataCore
    ContentMetadataCommon
    pqxx
    gtest
    gtest_main
    gmock
    gmock_main
)

add_test(NAME ContentMetadataRepository_Tests COMMAND ContentMetadataRepository_Tests)

install(TARGETS ContentMetadataRepository
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataRepository_Tests"
)

install(FILES "$<TARGET_PDB_FILE:ContentMetadataRepository>"
    DESTINATION "${CMAKE_BINARY_DIR}/tests/ContentMetadataRepository_Tests"
    OPTIONAL
)