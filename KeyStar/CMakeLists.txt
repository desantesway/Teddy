if(WIN32)
    add_executable(KeyStar WIN32)
    target_link_options(KeyStar PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
    )
else()
    add_executable(KeyStar)
endif()

target_sources(KeyStar PRIVATE    
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyStarApp.cpp
)

target_include_directories(KeyStar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/src
)

target_compile_definitions(KeyStar PRIVATE
    $<$<CONFIG:Debug>:TED_DEBUG>
    $<$<CONFIG:Release>:TED_RELEASE>
    $<$<CONFIG:Dist>:TED_DIST>
)

set_target_properties(KeyStar PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/KeyStar"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/KeyStar"
    PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bar)

