cmake_minimum_required(VERSION 3.12)

set(CORE_PROJECT_NAME GameEngineCore)
project(${CORE_PROJECT_NAME})

set(CORE_PUBLIC_INCLUDES
	includes/GameEngineCore/Application.hpp
	includes/GameEngineCore/Log.hpp
	includes/GameEngineCore/Event.hpp
)

set(CORE_PRIVATE_SOURCES
	src/GameEngineCore/Application.cpp
	src/GameEngineCore/Window.cpp
)

set(CORE_PRIVATE_INCLUDES 
	src/GameEngineCore/Window.hpp
)

add_library(${CORE_PROJECT_NAME} STATIC
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)
	
target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_20)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glad)