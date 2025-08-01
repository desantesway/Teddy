if(WIN32)
    add_executable(KeyStar WIN32)
    target_link_options(KeyStar PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
    )
else()
    add_executable(KeyStar)
endif()

set_property(TARGET KeyStar PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/KeyStar")

target_sources(KeyStar PRIVATE    
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyStarApp.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyStar2D.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyStar2D.cpp
)

target_include_directories(KeyStar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/src
)

target_compile_definitions(KeyStar PRIVATE
    $<$<CONFIG:Debug>:TED_DEBUG>
    $<$<CONFIG:Debug>:TED_PROFILE>
    $<$<CONFIG:Release>:TED_RELEASE>
    $<$<CONFIG:Dist>:TED_DIST>
)

set_target_properties(KeyStar PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/KeyStar"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/KeyStar"
    PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()
