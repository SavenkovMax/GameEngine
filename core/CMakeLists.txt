cmake_minimum_required(VERSION 3.20)

set(CORE_PROJECT_NAME GameEngineCore)
project(${CORE_PROJECT_NAME})

set(CORE_PUBLIC_INCLUDES
	includes/core/Core.hpp
	includes/core/MouseCodes.hpp
	includes/core/KeyCodes.hpp
	includes/core/Input.hpp
	includes/core/Application.hpp
	includes/core/Log.hpp
	includes/core/Event.hpp
	includes/core/Camera.hpp
	includes/core/LayerStack.hpp
	includes/core/Layer.hpp
	includes/core/Timestep.hpp
)

set(CORE_PRIVATE_INCLUDES
	src/core/Window.hpp
	src/core/Rendering/OpenGL/GLSLShaderProgram.hpp
	src/core/Rendering/OpenGL/VertexBuffer.hpp
	src/core/Rendering/OpenGL/VertexArray.hpp
	src/core/Rendering/OpenGL/IndexBuffer.hpp
	src/core/Rendering/OpenGL/RendererOGL.hpp
	src/core/Modules/UIModule.hpp
)

set(CORE_PRIVATE_SOURCES
	src/core/Layer.cpp
	src/core/LayerStack.cpp
	src/core/Application.cpp
	src/core/Window.cpp
	src/core/Camera.cpp
	src/core/Input.cpp
	src/core/Rendering/OpenGL/GLSLShaderProgram.cpp
	src/core/Rendering/OpenGL/VertexBuffer.cpp
	src/core/Rendering/OpenGL/VertexArray.cpp
	src/core/Rendering/OpenGL/IndexBuffer.cpp
	src/core/Rendering/OpenGL/RendererOGL.cpp
	src/core/Modules/UIModule.cpp
)

set(CORE_ALL_SOURCES
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)

add_library(${CORE_PROJECT_NAME} STATIC
	${CORE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_ALL_SOURCES}
)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_20)


add_subdirectory(../thirdparty/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../thirdparty/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../thirdparty/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../thirdparty/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glm)

set(IMGUI_INCLUDES
	../thirdparty/imgui/imgui.h
	../thirdparty/imgui/backends/imgui_impl_glfw.h
	../thirdparty/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../thirdparty/imgui/imgui.cpp
	../thirdparty/imgui/backends/imgui_impl_glfw.cpp
	../thirdparty/imgui/backends/imgui_impl_opengl3.cpp
	../thirdparty/imgui/imgui_draw.cpp
	../thirdparty/imgui/imgui_tables.cpp
	../thirdparty/imgui/imgui_widgets.cpp
	../thirdparty/imgui/imgui_demo.cpp
)

add_library(ImGui STATIC 
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../thirdparty)
target_include_directories(ImGui PUBLIC ../thirdparty/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE ImGui)