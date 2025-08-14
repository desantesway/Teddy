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
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tedditor)

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
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

if(Vulkan_FOUND)
    message(STATUS "Found Vulkan SDK: ${Vulkan_LIBRARY}")
    message(STATUS "Vulkan Include Dirs: ${Vulkan_INCLUDE_DIRS}")

    # Helper: create imported static library target with optional Debug (d suffix) variant
    function(add_vulkan_imported_lib target base)
        if(TARGET ${target})
            return()
        endif()
        if(NOT DEFINED VULKAN_SDK)
            set(VULKAN_SDK $ENV{VULKAN_SDK})
        endif()
        if(NOT VULKAN_SDK)
            message(WARNING "VULKAN_SDK environment variable not set; cannot add ${target}")
            return()
        endif()

        # Locate release & debug libs (debug have trailing 'd' on Windows)
        find_library(${target}_RELEASE NAMES ${base} HINTS "${VULKAN_SDK}/Lib" "${VULKAN_SDK}/Lib32")
        # Some libs do not ship a debug-suffixed variant; tolerate that.
        find_library(${target}_DEBUG   NAMES ${base}d ${base} HINTS "${VULKAN_SDK}/Lib" "${VULKAN_SDK}/Lib32")

        if(NOT ${target}_RELEASE AND NOT ${target}_DEBUG)
            message(WARNING "Could not find library for ${target} (base name: ${base})")
            return()
        endif()

        add_library(${target} STATIC IMPORTED GLOBAL)
        # Map Release-like configs plus custom Dist to release lib
        if(${target}_RELEASE)
            set_target_properties(${target} PROPERTIES
                IMPORTED_LOCATION_RELEASE "${${target}_RELEASE}"
                IMPORTED_LOCATION_RELWITHDEBINFO "${${target}_RELEASE}"
                IMPORTED_LOCATION_MINSIZEREL "${${target}_RELEASE}"
                IMPORTED_LOCATION_DIST "${${target}_RELEASE}"
            )
        endif()
        if(${target}_DEBUG)
            set_target_properties(${target} PROPERTIES
                IMPORTED_LOCATION_DEBUG "${${target}_DEBUG}"
            )
        endif()
    endfunction()

    # Create imported targets for SPIRV-Cross (core + GLSL), SPIRV-Tools, etc.
    # Adjust / add more (e.g. SPIRV-Tools-link) if you need them.
    add_vulkan_imported_lib(SPIRV_Cross_Core      spirv-cross-core)
    add_vulkan_imported_lib(SPIRV_Cross_GLSL      spirv-cross-glsl)
    add_vulkan_imported_lib(SPIRV_Tools           SPIRV-Tools)
    add_vulkan_imported_lib(SPIRV_Tools_Opt       SPIRV-Tools-opt)
    add_vulkan_imported_lib(VkLayer_utils         VkLayer_utils) # Only needed if writing validation/layer tools

    # Common include directory (already covered by Vulkan::Vulkan, but add explicitly for SPIRV-Cross headers)
    if(NOT VULKAN_SDK)
        set(VULKAN_SDK $ENV{VULKAN_SDK})
    endif()
    if(VULKAN_SDK)
        set(SPIRV_EXTRA_INCLUDE "${VULKAN_SDK}/Include")
    endif()

    foreach(tgt IN ITEMS Teddy Sandbox Tedditor)
        target_include_directories(${tgt} PRIVATE ${Vulkan_INCLUDE_DIRS} ${SPIRV_EXTRA_INCLUDE})
        # Core Vulkan + shaderc (shaderc_combined already pulls SPIRV-Tools internally)
        target_link_libraries(${tgt} PRIVATE Vulkan::Vulkan Vulkan::shaderc_combined)
        # Only link the extra libraries if they were found (TARGET exists)
        foreach(dep IN ITEMS SPIRV_Cross_Core SPIRV_Cross_GLSL SPIRV_Tools SPIRV_Tools_Opt VkLayer_utils)
            if(TARGET ${dep})
                target_link_libraries(${tgt} PRIVATE ${dep})
            endif()
        endforeach()
        target_compile_definitions(${tgt} PRIVATE TED_VULKAN_ENABLED)
    endforeach()

else()
    message(WARNING "Vulkan SDK not found. Vulkan rendering will be disabled.")
endif()

# Box2D
add_subdirectory(Teddy/vendor/box2d)

target_link_libraries(Teddy PRIVATE box2d::box2d)
target_link_libraries(Sandbox PRIVATE box2d::box2d)
target_link_libraries(Tedditor PRIVATE box2d::box2d)

set_target_properties(box2d PROPERTIES FOLDER "Dependencies")

# Engine linking
target_link_libraries(Sandbox PRIVATE Teddy)
target_link_libraries(Tedditor PRIVATE Teddy)

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