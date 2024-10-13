project(ContentMetadataCommon)

# Collect all source files from the DynamicLib folder
file(GLOB_RECURSE CONTENT_METADATA_COMMON_FILES "*.cpp" "*.h")

# Create a dynamic library
add_library(ContentMetadataCommon STATIC ${CONTENT_METADATA_COMMON_FILES})

# Add the shared include and external directory 
target_include_directories(ContentMetadataCommon PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/inih/cpp"
)


