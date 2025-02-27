#include <iostream>
#include <memory>

#include "core/Application.hpp"
#include "core/KeyCodes.hpp"
#include "core/Input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


class EngineEditor : public engine::Application {
	virtual void OnUpdate() override {
		if (engine::Input::IsKeyPressed(engine::KeyCode::W)) {
			camera_position[2] += 0.01f;
		}
		if (engine::Input::IsKeyPressed(engine::KeyCode::A)) {
			camera_position[0] += 0.01f;
		}
		if (engine::Input::IsKeyPressed(engine::KeyCode::S)) {
			camera_position[2] -= 0.01f;
		}
		if (engine::Input::IsKeyPressed(engine::KeyCode::D)) {
			camera_position[0] -= 0.01f;
		}
		if (engine::Input::IsKeyPressed(engine::KeyCode::Q)) {
			camera_position[1] += 0.01f;
		}
		if (engine::Input::IsKeyPressed(engine::KeyCode::E)) {
			camera_position[1] -= 0.01f;
		}
	}

	virtual void OnUIDraw() override {
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Camera position", glm::value_ptr(camera_position), -10.f, 10.f);
		ImGui::SliderFloat3("Camera rotation", glm::value_ptr(camera_rotation), 0.f, 360.f);
		ImGui::Checkbox("Perspective camera", &perspective_camera);
		ImGui::End();
	}

	int frame = 0;
};


int main() {
	auto myApp = std::make_unique<EngineEditor>();

	int returnCode = myApp->Run(1024, 768, "Engine editor");

	std::cin.get();

	return returnCode;
}