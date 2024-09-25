cmake_minimum_required(VERSION 3.8)

# Enable XAML hot-reloading for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("ContentMetadataService")

# Set the C++ standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn off specific compiler extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subprojects (libraries)
add_subdirectory(ContentMetadataApi)
add_subdirectory(ContentMetadataCore)
add_subdirectory(ContentMetadataRepository)
add_subdirectory(tests)

# Collect all source files from the src folder
file(GLOB_RECURSE EXECUTABLE_FILES "src/*.cpp" "src/*.h")

# Add the source files to the executable for this project. (Defines this executable)
add_executable(ContentMetadataService ${EXECUTABLE_FILES})

# Add the shared include and external directory 
target_include_directories(ContentMetadataService PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/include"
)

# Link the libraries with the project
target_link_libraries(ContentMetadataService PRIVATE ContentMetadataApi)
target_link_libraries(ContentMetadataService PRIVATE ContentMetadataCore)
link_directories("${CMAKE_BINARY_DIR}/ContentMetadataCore")
link_directories("${CMAKE_BINARY_DIR}/ContentMetadataApi")


#add_custom_command(TARGET ContentMetadataService POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH};${CMAKE_CURRENT_BINARY_DIR}/ContentMetadataApi"
#    $<TARGET_FILE:ContentMetadataService>)

# Install the main executable (optional, but often used in projects)
install(TARGETS ContentMetadataApi
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}"
)

# Install the .pdb files (symbols for debugging)
install(FILES "$<TARGET_PDB_FILE:ContentMetadataApi>"
    DESTINATION "${CMAKE_BINARY_DIR}"
    OPTIONAL
)

message("CMAKE_BINARY_DIR is set to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")