project(ContentMetadataRepository)

# Collect all source files from the DynamicLib folder
file(GLOB_RECURSE CONTENT_METADATA_REPOSITORY_FILES "*.cpp" "*.h")

# Create a dynamic library
add_library(ContentMetadataRepository SHARED ${CONTENT_METADATA_REPOSITORY_FILES})

# Add the definition for the BUILD_DLL macro to indicate we are compiling the DLL
target_compile_definitions(ContentMetadataRepository PRIVATE BUILD_CONTENT_METADATA_REPOSITORY)

target_include_directories(ContentMetadataRepository PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

