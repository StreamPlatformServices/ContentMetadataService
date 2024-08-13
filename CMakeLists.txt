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
#add_subdirectory(StaticLib)

# Collect all source files from the src folder
file(GLOB_RECURSE EXECUTABLE_FILES "src/*.cpp" "src/*.h")

# Add the source files to the executable for this project.
add_executable(ContentMetadataService ${EXECUTABLE_FILES})

# Add the shared include directory
target_include_directories(ContentMetadataApi PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Static library - collect all files from the StaticLib folder
#file(GLOB_RECURSE STATIC_LIB_FILES "StaticLib/*.cpp" "StaticLib/*.h")
#add_library(MyStaticLib STATIC ${STATIC_LIB_FILES})

# Link the libraries with the project
target_link_libraries(ContentMetadataService PRIVATE ContentMetadataApi)

# TODO: Add tests and install targets as needed.
