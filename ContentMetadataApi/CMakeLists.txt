project(ContentMetadataApi)
 
# Add the shared include directory (only for testing purpose)
message("CMAKE_SOURCE_DIR is set to: ${CMAKE_CURRENT_SOURCE_DIR}")



# Collect all source files from the DynamicLib folder
file(GLOB_RECURSE CONTENT_METADATA_API_FILES "*.cpp" "*.h")

# Create a dynamic library
add_library(ContentMetadataApi SHARED ${CONTENT_METADATA_API_FILES})

# Add the definition for the BUILD_DLL macro to indicate we are compiling the DLL
target_compile_definitions(ContentMetadataApi PRIVATE BUILD_DLL)

target_include_directories(ContentMetadataApi PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Set the C++ standard (if required)
set_property(TARGET ContentMetadataApi PROPERTY CXX_STANDARD 20)
