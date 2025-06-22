cmake_minimum_required(VERSION 3.20)

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
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(OUTPUT_INT_DIR "${CMAKE_BINARY_DIR}/bin-int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

find_package(OpenGL REQUIRED)

set(GLAD_SRC Teddy/vendor/glad/src/glad.c)
set(GLAD_INC Teddy/vendor/glad/include)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${GLAD_INC})

#sdl
set(SDL_TEST OFF)

#add_subdirectory(Teddy/vendor/SDL)
include_directories(Teddy/vendor/SDL/include)

# Add the precompiled library
add_library(SDL3::SDL3 STATIC IMPORTED)
set_target_properties(SDL3::SDL3 PROPERTIES
    IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Teddy/vendor/SDL/Debug/SDL3.lib"
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Teddy/vendor/SDL/Release/SDL3.lib"
    IMPORTED_LOCATION_DIST "${CMAKE_SOURCE_DIR}/Teddy/vendor/SDL/Dist/SDL3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Teddy/vendor/SDL/include"
)

add_subdirectory(Teddy)
add_subdirectory(KeyStar)

add_subdirectory(Teddy/vendor/spdlog)

target_link_libraries(Teddy PRIVATE glad)
target_link_libraries(KeyStar PRIVATE glad)

target_link_libraries(Teddy PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(KeyStar PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

target_link_libraries(Teddy PRIVATE SDL3::SDL3)
target_link_libraries(KeyStar PRIVATE SDL3::SDL3)

target_link_libraries(KeyStar PRIVATE Teddy)