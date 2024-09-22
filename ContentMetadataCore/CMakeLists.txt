project(ContentMetadataCore)

# Collect all source files from the DynamicLib folder
file(GLOB_RECURSE CONTENT_METADATA_CORE_FILES "*.cpp" "*.h")

# Create a dynamic library
add_library(ContentMetadataCore SHARED ${CONTENT_METADATA_CORE_FILES})

# Add the definition for the BUILD_DLL macro to indicate we are compiling the DLL
target_compile_definitions(ContentMetadataCore PRIVATE BUILD_CONTENT_METADATA_CORE)

# Add the shared include and external directory 
target_include_directories(ContentMetadataCore PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/include"
)

