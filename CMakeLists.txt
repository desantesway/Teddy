cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE) # default to Debug if not specified
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

project(Teddy)

# if windows add definitions for windows platform and set MultiThreaded with /utf-8 for spdlog
if(WIN32)
    add_definitions(-DTED_PLATFORM_WINDOWS -DBUILD_SHARED_LIBS=OFF)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(OUTPUT_INT_DIR "${CMAKE_BINARY_DIR}/bin-int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# Engine & Application
add_subdirectory(Teddy)
add_subdirectory(Sandbox)
add_subdirectory(Tedditor)
add_subdirectory(Cuphead)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Cuphead)

add_subdirectory(Teddy/vendor)

# Engine linking
target_link_libraries(Sandbox PRIVATE Teddy)
target_link_libraries(Tedditor PRIVATE Teddy)
target_link_libraries(Cuphead PRIVATE Teddy)

# VS folders
set_target_properties(Teddy PROPERTIES FOLDER "Engine")
set_target_properties(Cuphead PROPERTIES FOLDER "Engine/Game")
set_target_properties(Sandbox PROPERTIES FOLDER "Engine/Game")
set_target_properties(Tedditor PROPERTIES FOLDER "Engine/Game")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)