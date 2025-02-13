cmake_minimum_required(VERSION 3.20)

project(GameEngine)

add_subdirectory(Core)
add_subdirectory(Editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameEngineEditor)