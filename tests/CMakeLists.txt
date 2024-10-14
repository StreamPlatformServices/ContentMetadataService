cmake_minimum_required(VERSION 3.15)
project(ContentMetadataService_Tests)

enable_testing()

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

add_subdirectory(ContentMetadataApi_Tests) 
add_subdirectory(ContentMetadataRepository_Tests) 
add_subdirectory(ContentMetadataCommon_Tests)
