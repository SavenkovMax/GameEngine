cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME GameEngine)

add_subdirectory(Editor)
add_subdirectory(Core)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameEngineEditor)