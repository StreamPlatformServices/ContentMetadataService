project(ContentMetadataApi)
 
#(only for testing purpose)
message("CMAKE_SOURCE_DIR is set to: ${CMAKE_CURRENT_SOURCE_DIR}")

# Collect all source files from the DynamicLib folder
file(GLOB_RECURSE CONTENT_METADATA_API_FILES "*.cpp" "*.hpp")

# Create a dynamic library (This target is defined with this command)
add_library(ContentMetadataApi SHARED ${CONTENT_METADATA_API_FILES})

# Add the definition for the BUILD_DLL macro to indicate we are compiling the DLL
target_compile_definitions(ContentMetadataApi PRIVATE BUILD_CONTENT_METADATA_API)

# Add the shared include and external directory 
target_include_directories(ContentMetadataApi PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/lib"
)

#link lib
target_link_libraries(ContentMetadataApi PRIVATE ContentMetadataCore)
link_directories("${CMAKE_BINARY_DIR}/../ContentMetadataCore")

#target_link_libraries(ContentMetadataApi PRIVATE restinio)

# Set the C++ standard (if required)
#set_property(TARGET ContentMetadataApi PROPERTY CXX_STANDARD 20)



