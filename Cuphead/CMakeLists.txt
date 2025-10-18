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

                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/GameScenes.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/GameScenes.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/MainTitleScene.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/MainTitleScene.h

                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/MainMenuScene.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/MainMenuScene.h

                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/DlcMenu.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/PlayMenu.cpp

                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/OptionsMenu.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/VisualOptions.cpp

                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/TransitionScenes.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/TransitionScenes.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/Slider.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scenes/Slider.h

                                    # Editor
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/EditorLayer.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/EditorLayer.cpp

                                    # Level
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/LevelScene.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/LevelScene.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/Player.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/Player.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/CloudPlatform.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/CloudPlatform.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/LevelCategories.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/PauseMenu.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Level/PauseMenu.cpp
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
