# sdl
set(SDL_SHARED OFF)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(SDL)
include_directories(SDL/include)

target_link_libraries(Teddy PUBLIC SDL3-static)

get_property(SDL3_TARGETS DIRECTORY SDL PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN LISTS SDL3_TARGETS)
    set_target_properties(${target} PROPERTIES FOLDER "Dependencies/Window")
endforeach()

# freetype
add_subdirectory(freetype)
set_target_properties(freetype PROPERTIES FOLDER "Dependencies/Text")

#target_link_libraries(freetype PUBLIC zlib)

# TODO: Remove files i dont use from msdfgen
# msdfgen
include_directories(msdf-atlas-gen/msdfgen)

set(MSDF_SRC
    msdf-atlas-gen/msdfgen/main.cpp
    msdf-atlas-gen/msdfgen/msdfgen.h
    msdf-atlas-gen/msdfgen/msdfgen-ext.h
    msdf-atlas-gen/msdfgen/resource.h
    msdf-atlas-gen/msdfgen/ext/import-font.cpp
    msdf-atlas-gen/msdfgen/ext/import-font.h
    msdf-atlas-gen/msdfgen/ext/import-svg.cpp
    msdf-atlas-gen/msdfgen/ext/import-svg.h
    msdf-atlas-gen/msdfgen/ext/resolve-shape-geometry.cpp
    msdf-atlas-gen/msdfgen/ext/resolve-shape-geometry.h
    msdf-atlas-gen/msdfgen/ext/save-png.cpp
    msdf-atlas-gen/msdfgen/ext/save-png.h
    msdf-atlas-gen/msdfgen/core/arithmetics.hpp
    msdf-atlas-gen/msdfgen/core/base.h
    msdf-atlas-gen/msdfgen/core/bitmap-interpolation.hpp
    msdf-atlas-gen/msdfgen/core/Bitmap.h
    msdf-atlas-gen/msdfgen/core/Bitmap.hpp
    msdf-atlas-gen/msdfgen/core/BitmapRef.hpp
    msdf-atlas-gen/msdfgen/core/contour-combiners.cpp
    msdf-atlas-gen/msdfgen/core/contour-combiners.h
    msdf-atlas-gen/msdfgen/core/Contour.cpp
    msdf-atlas-gen/msdfgen/core/Contour.h
    msdf-atlas-gen/msdfgen/core/convergent-curve-ordering.cpp
    msdf-atlas-gen/msdfgen/core/convergent-curve-ordering.h
    msdf-atlas-gen/msdfgen/core/DistanceMapping.cpp
    msdf-atlas-gen/msdfgen/core/DistanceMapping.h
    msdf-atlas-gen/msdfgen/core/edge-coloring.cpp
    msdf-atlas-gen/msdfgen/core/edge-coloring.h
    msdf-atlas-gen/msdfgen/core/edge-segments.cpp
    msdf-atlas-gen/msdfgen/core/edge-segments.h
    msdf-atlas-gen/msdfgen/core/edge-selectors.cpp
    msdf-atlas-gen/msdfgen/core/edge-selectors.h
    msdf-atlas-gen/msdfgen/core/EdgeColor.h
    msdf-atlas-gen/msdfgen/core/EdgeHolder.cpp
    msdf-atlas-gen/msdfgen/core/EdgeHolder.h
    msdf-atlas-gen/msdfgen/core/equation-solver.cpp
    msdf-atlas-gen/msdfgen/core/equation-solver.h
    msdf-atlas-gen/msdfgen/core/export-svg.cpp
    msdf-atlas-gen/msdfgen/core/export-svg.h
    msdf-atlas-gen/msdfgen/core/generator-config.h
    msdf-atlas-gen/msdfgen/core/msdf-error-correction.cpp
    msdf-atlas-gen/msdfgen/core/msdf-error-correction.h
    msdf-atlas-gen/msdfgen/core/MSDFErrorCorrection.cpp
    msdf-atlas-gen/msdfgen/core/MSDFErrorCorrection.h
    msdf-atlas-gen/msdfgen/core/msdfgen.cpp
    msdf-atlas-gen/msdfgen/core/pixel-conversion.hpp
    msdf-atlas-gen/msdfgen/core/Projection.cpp
    msdf-atlas-gen/msdfgen/core/Projection.h
    msdf-atlas-gen/msdfgen/core/Range.hpp
    msdf-atlas-gen/msdfgen/core/rasterization.cpp
    msdf-atlas-gen/msdfgen/core/rasterization.h
    msdf-atlas-gen/msdfgen/core/render-sdf.cpp
    msdf-atlas-gen/msdfgen/core/render-sdf.h
    msdf-atlas-gen/msdfgen/core/save-bmp.cpp
    msdf-atlas-gen/msdfgen/core/save-bmp.h
    msdf-atlas-gen/msdfgen/core/save-fl32.cpp
    msdf-atlas-gen/msdfgen/core/save-fl32.h
    msdf-atlas-gen/msdfgen/core/save-rgba.cpp
    msdf-atlas-gen/msdfgen/core/save-rgba.h
    msdf-atlas-gen/msdfgen/core/save-tiff.cpp
    msdf-atlas-gen/msdfgen/core/save-tiff.h
    msdf-atlas-gen/msdfgen/core/Scanline.cpp
    msdf-atlas-gen/msdfgen/core/Scanline.h
    msdf-atlas-gen/msdfgen/core/sdf-error-estimation.cpp
    msdf-atlas-gen/msdfgen/core/sdf-error-estimation.h
    msdf-atlas-gen/msdfgen/core/SDFTransformation.h
    msdf-atlas-gen/msdfgen/core/shape-description.cpp
    msdf-atlas-gen/msdfgen/core/shape-description.h
    msdf-atlas-gen/msdfgen/core/Shape.cpp
    msdf-atlas-gen/msdfgen/core/Shape.h
    msdf-atlas-gen/msdfgen/core/ShapeDistanceFinder.h
    msdf-atlas-gen/msdfgen/core/ShapeDistanceFinder.hpp
    msdf-atlas-gen/msdfgen/core/SignedDistance.hpp
    msdf-atlas-gen/msdfgen/core/Vector2.hpp
)

add_library(msdfgen STATIC ${MSDF_SRC})

target_compile_definitions(msdfgen PUBLIC MSDFGEN_PUBLIC=)

target_include_directories(msdfgen PUBLIC 
            msdf-atlas-gen/msdfgen/
            msdf-atlas-gen/msdfgen/core
            msdf-atlas-gen/msdfgen/ext
)

target_link_libraries(msdfgen PUBLIC freetype)

set_target_properties(msdfgen PROPERTIES FOLDER "Dependencies/Text")

# artery font
include_directories(msdf-atlas-gen/artery-font-format/artery-font)

set(ARTERY_FONT_SRC
    msdf-atlas-gen/artery-font-format/artery-font/artery-font.h
    msdf-atlas-gen/artery-font-format/artery-font/crc32.h
    msdf-atlas-gen/artery-font-format/artery-font/crc32.hpp
    msdf-atlas-gen/artery-font-format/artery-font/enums.h
    msdf-atlas-gen/artery-font-format/artery-font/serialization.h
    msdf-atlas-gen/artery-font-format/artery-font/serialization.hpp
    msdf-atlas-gen/artery-font-format/artery-font/std-artery-font.h
    msdf-atlas-gen/artery-font-format/artery-font/stdio-serialization.h
    msdf-atlas-gen/artery-font-format/artery-font/stdio-serialization.hpp
    msdf-atlas-gen/artery-font-format/artery-font/structures.h
    msdf-atlas-gen/artery-font-format/artery-font/types.h
)

add_library(artery-font INTERFACE)
target_sources(artery-font INTERFACE ${ARTERY_FONT_SRC})
target_include_directories(artery-font INTERFACE 
    msdf-atlas-gen/artery-font-format/
    msdf-atlas-gen/artery-font-format/artery-font/
)

set_target_properties(artery-font PROPERTIES FOLDER "Dependencies/Text")

# msdf-atlas-gen
include_directories(msdf-atlas-gen)

set(MSDF_ATLAS_SRC
    msdf-atlas-gen/resource.h
    msdf-atlas-gen/msdf-atlas-gen/artery-font-export.cpp
    msdf-atlas-gen/msdf-atlas-gen/artery-font-export.h
    msdf-atlas-gen/msdf-atlas-gen/AtlasGenerator.h
    msdf-atlas-gen/msdf-atlas-gen/AtlasStorage.h
    msdf-atlas-gen/msdf-atlas-gen/bitmap-blit.cpp
    msdf-atlas-gen/msdf-atlas-gen/bitmap-blit.h
    msdf-atlas-gen/msdf-atlas-gen/BitmapAtlasStorage.h
    msdf-atlas-gen/msdf-atlas-gen/BitmapAtlasStorage.hpp
    msdf-atlas-gen/msdf-atlas-gen/charset-parser.cpp
    msdf-atlas-gen/msdf-atlas-gen/Charset.cpp
    msdf-atlas-gen/msdf-atlas-gen/Charset.h
    msdf-atlas-gen/msdf-atlas-gen/csv-export.cpp
    msdf-atlas-gen/msdf-atlas-gen/csv-export.h
    msdf-atlas-gen/msdf-atlas-gen/DynamicAtlas.h
    msdf-atlas-gen/msdf-atlas-gen/DynamicAtlas.hpp
    msdf-atlas-gen/msdf-atlas-gen/FontGeometry.cpp
    msdf-atlas-gen/msdf-atlas-gen/FontGeometry.h
    msdf-atlas-gen/msdf-atlas-gen/glyph-generators.cpp
    msdf-atlas-gen/msdf-atlas-gen/glyph-generators.h
    msdf-atlas-gen/msdf-atlas-gen/GlyphBox.h
    msdf-atlas-gen/msdf-atlas-gen/GlyphGeometry.cpp
    msdf-atlas-gen/msdf-atlas-gen/GlyphGeometry.h
    msdf-atlas-gen/msdf-atlas-gen/GridAtlasPacker.cpp
    msdf-atlas-gen/msdf-atlas-gen/GridAtlasPacker.h
    msdf-atlas-gen/msdf-atlas-gen/image-encode.cpp
    msdf-atlas-gen/msdf-atlas-gen/image-encode.h
    msdf-atlas-gen/msdf-atlas-gen/image-save.h
    msdf-atlas-gen/msdf-atlas-gen/image-save.hpp
    msdf-atlas-gen/msdf-atlas-gen/ImmediateAtlasGenerator.h
    msdf-atlas-gen/msdf-atlas-gen/ImmediateAtlasGenerator.hpp
    msdf-atlas-gen/msdf-atlas-gen/json-export.cpp
    msdf-atlas-gen/msdf-atlas-gen/json-export.h
    msdf-atlas-gen/msdf-atlas-gen/main.cpp
    msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h
    msdf-atlas-gen/msdf-atlas-gen/Padding.cpp
    msdf-atlas-gen/msdf-atlas-gen/Padding.h
    msdf-atlas-gen/msdf-atlas-gen/rectangle-packing.h
    msdf-atlas-gen/msdf-atlas-gen/rectangle-packing.hpp
    msdf-atlas-gen/msdf-atlas-gen/Rectangle.h
    msdf-atlas-gen/msdf-atlas-gen/RectanglePacker.cpp
    msdf-atlas-gen/msdf-atlas-gen/RectanglePacker.h
    msdf-atlas-gen/msdf-atlas-gen/Remap.h
    msdf-atlas-gen/msdf-atlas-gen/shadron-preview-generator.cpp
    msdf-atlas-gen/msdf-atlas-gen/shadron-preview-generator.h
    msdf-atlas-gen/msdf-atlas-gen/size-selectors.cpp
    msdf-atlas-gen/msdf-atlas-gen/size-selectors.h
    msdf-atlas-gen/msdf-atlas-gen/TightAtlasPacker.cpp
    msdf-atlas-gen/msdf-atlas-gen/TightAtlasPacker.h
    msdf-atlas-gen/msdf-atlas-gen/types.h
    msdf-atlas-gen/msdf-atlas-gen/utf8.cpp
    msdf-atlas-gen/msdf-atlas-gen/utf8.h
    msdf-atlas-gen/msdf-atlas-gen/utils.hpp
    msdf-atlas-gen/msdf-atlas-gen/Workload.cpp
    msdf-atlas-gen/msdf-atlas-gen/Workload.h
)

add_library(msdf-atlas-gen STATIC ${MSDF_ATLAS_SRC})

target_include_directories(msdf-atlas-gen PUBLIC 
            msdf-atlas-gen 
            msdf-atlas-gen/msdf-atlas-gen/ 
)

target_link_libraries(msdf-atlas-gen PUBLIC artery-font)
target_link_libraries(msdf-atlas-gen PUBLIC msdfgen)

target_link_libraries(Teddy PUBLIC msdf-atlas-gen)

set_target_properties(msdf-atlas-gen PROPERTIES FOLDER "Dependencies/Text")

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