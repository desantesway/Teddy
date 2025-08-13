if(WIN32)
    add_executable(Sandbox WIN32)
    target_link_options(Sandbox PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
    )
else()
    add_executable(Sandbox)
endif()

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")

target_sources(Sandbox PRIVATE    
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/SandboxApp.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sandbox2D.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sandbox2D.cpp
)

target_include_directories(Sandbox PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/src
)

target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:TED_DEBUG>
    $<$<CONFIG:Release>:TED_RELEASE>
    $<$<CONFIG:Dist>:TED_DIST>
)

set_target_properties(Sandbox PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Sandbox"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Sandbox"
    PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()
