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

#add_custom_command(TARGET KeyStar POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        $<TARGET_FILE:SDL3-shared>
#        "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
#    COMMENT "Copying SDL3.dll to output directory"
#)

add_custom_command(TARGET KeyStar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/vendor/SDL/$<CONFIG>/SDL3.dll"
        "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
    COMMENT "Copying SDL3.dll to output directory"
)

add_custom_command(TARGET KeyStar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Teddy>
        "${OUTPUT_DIR}/KeyStar/$<CONFIG>"
    COMMENT "Copying Teddy DLL to KeyStar directory for $<CONFIG> build"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bar)

