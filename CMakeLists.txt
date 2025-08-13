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
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(OUTPUT_INT_DIR "${CMAKE_BINARY_DIR}/bin-int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# Engine & Application
add_subdirectory(Teddy)
add_subdirectory(Sandbox)
add_subdirectory(Tedditor)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

#sdl
set(SDL3_TEST OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(Teddy/vendor/SDL)
include_directories(Teddy/vendor/SDL/include)

target_link_libraries(Teddy PRIVATE SDL3-static)
target_link_libraries(Sandbox PRIVATE SDL3-static)
target_link_libraries(Tedditor PRIVATE SDL3-static)

# SpdLog
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries")
add_subdirectory(Teddy/vendor/spdlog)

target_link_libraries(Teddy PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(Sandbox PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(Tedditor PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# Glad
find_package(OpenGL REQUIRED)

set(GLAD_SRC Teddy/vendor/glad/src/glad.c)
set(GLAD_INC Teddy/vendor/glad/include)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${GLAD_INC})

target_link_libraries(Teddy PRIVATE glad)
target_link_libraries(Sandbox PRIVATE glad)
target_link_libraries(Tedditor PRIVATE glad)

# ImGui
include_directories(Teddy/vendor/ImGui)

set(IMGUI_SRC
    Teddy/vendor/ImGui/backends/imgui_impl_sdl3.cpp
    Teddy/vendor/ImGui/backends/imgui_impl_opengl3.cpp
    Teddy/vendor/ImGui/imconfig.h
	Teddy/vendor/ImGui/imgui.h
	Teddy/vendor/ImGui/imgui.cpp
    Teddy/vendor/ImGui/imgui_draw.cpp
	Teddy/vendor/ImGui/imgui_internal.h
	Teddy/vendor/ImGui/imgui_tables.cpp
	Teddy/vendor/ImGui/imgui_widgets.cpp
	Teddy/vendor/ImGui/imstb_rectpack.h
	Teddy/vendor/ImGui/imstb_textedit.h
	Teddy/vendor/ImGui/imstb_truetype.h
	Teddy/vendor/ImGui/imgui_demo.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC Teddy/vendor/ImGui Teddy/vendor/ImGui/backends)

target_link_libraries(Teddy PRIVATE imgui)
target_link_libraries(Sandbox PRIVATE imgui)
target_link_libraries(Tedditor PRIVATE imgui)

# GLM
target_include_directories(Teddy PRIVATE Teddy/vendor/GLM)
target_include_directories(Sandbox PRIVATE Teddy/vendor/GLM)
target_include_directories(Tedditor PRIVATE Teddy/vendor/GLM)

# stb_image
target_include_directories(Teddy PRIVATE Teddy/vendor/stb_image)
target_include_directories(Sandbox PRIVATE Teddy/vendor/stb_image)
target_include_directories(Tedditor PRIVATE Teddy/vendor/stb_image)

# rtmidi
add_subdirectory(Teddy/vendor/rtmidi)
target_include_directories(Teddy PRIVATE Teddy/vendor/rtmidi/include)
target_include_directories(Sandbox PRIVATE Teddy/vendor/rtmidi/include)
target_include_directories(Tedditor PRIVATE Teddy/vendor/rtmidi/include)

target_link_libraries(Teddy PRIVATE rtmidi)
target_link_libraries(Sandbox PRIVATE rtmidi)
target_link_libraries(Tedditor PRIVATE rtmidi)

target_link_libraries(Sandbox PRIVATE Teddy)
target_link_libraries(Tedditor PRIVATE Teddy)

# entt
target_include_directories(Teddy PRIVATE Teddy/vendor/entt/single_include)
target_include_directories(Sandbox PRIVATE Teddy/vendor/entt/single_include)
target_include_directories(Tedditor PRIVATE Teddy/vendor/entt/single_include)

# yaml
add_subdirectory(Teddy/vendor/yaml-cpp)
target_link_libraries(Teddy PUBLIC yaml-cpp::yaml-cpp)
#target_link_libraries(Sandbox PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(Tedditor PUBLIC yaml-cpp::yaml-cpp)

# vulkan sdk
find_package(Vulkan REQUIRED FATAL_ERROR)

if(Vulkan_FOUND)
    message(STATUS "Found Vulkan SDK: ${Vulkan_LIBRARY}")
    message(STATUS "Vulkan Headers: ${Vulkan_INCLUDE_DIRS}")
    
    # Link Vulkan to all targets
    target_include_directories(Teddy PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_include_directories(Sandbox PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_include_directories(Tedditor PRIVATE ${Vulkan_INCLUDE_DIRS})
    
    target_link_libraries(Teddy PRIVATE ${Vulkan_LIBRARIES})
    target_link_libraries(Sandbox PRIVATE ${Vulkan_LIBRARIES})
    target_link_libraries(Tedditor PRIVATE ${Vulkan_LIBRARIES})
    
    # Add Vulkan compile definition
    target_compile_definitions(Teddy PRIVATE TED_VULKAN_ENABLED)
    target_compile_definitions(Sandbox PRIVATE TED_VULKAN_ENABLED)
    target_compile_definitions(Tedditor PRIVATE TED_VULKAN_ENABLED)
else()
    message(WARNING "Vulkan SDK not found. Vulkan rendering will be disabled.")
endif()

# shaderc
#add_subdirectory(Teddy/vendor/shaderc)

# VS folders
set_target_properties(Teddy PROPERTIES FOLDER "Game")
set_target_properties(Sandbox PROPERTIES FOLDER "Game")
set_target_properties(Tedditor PROPERTIES FOLDER "Game")

set_target_properties(imgui PROPERTIES FOLDER "Dependencies")
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

set_target_properties(yaml-cpp PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-parse PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-read PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-sandbox PROPERTIES FOLDER "Dependencies")

get_property(RTMIDI_TARGETS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Teddy/vendor/rtmidi PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN LISTS RTMIDI_TARGETS)
    set_target_properties(${target} PROPERTIES FOLDER "Dependencies")
endforeach()

set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")

get_property(SDL3_TARGETS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Teddy/vendor/SDL PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN LISTS SDL3_TARGETS)
    set_target_properties(${target} PROPERTIES FOLDER "Dependencies")
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)