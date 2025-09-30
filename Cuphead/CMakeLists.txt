if(WIN32)
    add_executable(Cuphead WIN32)
    target_link_options(Cuphead PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
    )
else()
    add_executable(Cuphead)
endif()

set_property(TARGET Cuphead PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Cuphead")

target_sources(Cuphead PRIVATE    
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/CupheadApp.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/CupheadMainLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/CupheadMainLayer.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/OverlayLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/OverlayLayer.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/StartLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/StartLayer.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/EndLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/EndLayer.cpp

                                    # Editor
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/EditorLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/EditorLayer.cpp
)

target_include_directories(Cuphead PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Teddy/src
)

target_compile_definitions(Cuphead PRIVATE
    $<$<CONFIG:Debug>:TED_DEBUG>
    $<$<CONFIG:Release>:TED_RELEASE>
    $<$<CONFIG:Dist>:TED_DIST>
)

set_target_properties(Cuphead PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Cuphead"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/Cuphead"
    PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()

target_link_libraries(Cuphead PRIVATE TedditorLib)
