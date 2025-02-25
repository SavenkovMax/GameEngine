#pragma once

struct GLFWwindow;

namespace engine {

	class UIModule {
	public:
		static void OnCreateWindow(GLFWwindow* window);
		static void OnCloseWindow();
		static void OnUIDrawBegin();
		static void OnUIDrawEnd();
		static void ShowExampleAppDockSpace(bool* open);

	private:
	};

} // namespace engine