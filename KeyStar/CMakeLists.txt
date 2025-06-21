add_executable(KeyStar)

target_sources(KeyStar PRIVATE    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyStarApp.cpp
)

target_include_directories(KeyStar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/vendor/spdlog/include
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

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_custom_command(TARGET KeyStar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Teddy>
        "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
    COMMENT "Copying Teddy DLL to KeyStar directory"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bar)