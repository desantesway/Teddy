#sdl
set(SDL_SHARED OFF)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(SDL)
include_directories(SDL/include)

target_link_libraries(Teddy PUBLIC SDL3-static)

get_property(SDL3_TARGETS DIRECTORY SDL PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN LISTS SDL3_TARGETS)
    set_target_properties(${target} PROPERTIES FOLDER "Dependencies/Window")
endforeach()

# ImGui
include_directories(ImGui)

set(IMGUI_SRC
    ImGui/backends/imgui_impl_sdl3.cpp
    ImGui/backends/imgui_impl_opengl3.cpp
    ImGui/imconfig.h
	ImGui/imgui.h
	ImGui/imgui.cpp
    ImGui/imgui_draw.cpp
	ImGui/imgui_internal.h
	ImGui/imgui_tables.cpp
	ImGui/imgui_widgets.cpp
	ImGui/imstb_rectpack.h
	ImGui/imstb_textedit.h
	ImGui/imstb_truetype.h
	ImGui/imgui_demo.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ImGui ImGui/backends)

target_link_libraries(Teddy PUBLIC imgui)

set_target_properties(imgui PROPERTIES FOLDER "Dependencies/Gui")

# SpdLog
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries")
add_subdirectory(spdlog)

target_link_libraries(Teddy PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

set_target_properties(spdlog PROPERTIES FOLDER "Dependencies/Log")

# Glad
find_package(OpenGL REQUIRED)

set(GLAD_SRC glad/src/glad.c)
set(GLAD_INC glad/include)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${GLAD_INC})

target_link_libraries(Teddy PUBLIC glad)

set_target_properties(glad PROPERTIES FOLDER "Dependencies/OpenGL")

# GLM
target_include_directories(Teddy PUBLIC GLM)

# stb_image
target_include_directories(Teddy PUBLIC stb_image)

# rtmidi
add_subdirectory(rtmidi)
target_include_directories(Teddy PUBLIC rtmidi/include)

target_link_libraries(Teddy PUBLIC rtmidi)

get_property(RTMIDI_TARGETS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rtmidi PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN LISTS RTMIDI_TARGETS)
    set_target_properties(${target} PROPERTIES FOLDER "Dependencies/RtMidi")
endforeach()

# entt
target_include_directories(Teddy PUBLIC entt/single_include)

# yaml
add_subdirectory(yaml-cpp)
target_link_libraries(Teddy PUBLIC yaml-cpp::yaml-cpp)

set_target_properties(yaml-cpp PROPERTIES FOLDER "Dependencies/Yaml")
set_target_properties(yaml-cpp-parse PROPERTIES FOLDER "Dependencies/Yaml")
set_target_properties(yaml-cpp-read PROPERTIES FOLDER "Dependencies/Yaml")
set_target_properties(yaml-cpp-sandbox PROPERTIES FOLDER "Dependencies/Yaml")

# Box2D
add_subdirectory(box2d)

target_link_libraries(Teddy PUBLIC box2d::box2d)

set_target_properties(box2d PROPERTIES FOLDER "Dependencies/Physics")

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