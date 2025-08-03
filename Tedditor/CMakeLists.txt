if(WIN32)
    add_executable(Tedditor WIN32)
    target_link_options(Tedditor PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
    )
else()
    add_executable(Tedditor)
endif()

set_property(TARGET Tedditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Tedditor")

target_sources(Tedditor PRIVATE    
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/TedditorApp.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/EditorLayer.h
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/EditorLayer.cpp
)

target_include_directories(Tedditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/src
)

target_compile_definitions(Tedditor PRIVATE
    $<$<CONFIG:Debug>:TED_DEBUG>
    $<$<CONFIG:Release>:TED_RELEASE>
    $<$<CONFIG:Dist>:TED_DIST>
)

set_target_properties(Tedditor PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Tedditor"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Tedditor"
    PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()
