#pragma once

#include "../Input/Input.h"
// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <windows.h>

#define _OPENGL

#ifdef _OPENGL
namespace RaisinEng
{
	void mouse_press_callback(int _button, int _action, int _mods);

	void Editor_Init();
	void Editor_Loop();
	void Editor_Close();

	inline GLFWwindow* m_window = nullptr;
	inline bool Foolscreen = true;
	inline void mouse_press_callback(GLFWwindow* _window, int _button, int _action, int _mods)
	{
		mouse_press_callback(_button, _action, _mods);
	}

	inline void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
	{
		glViewport(0, 0, _width, _height);
	}

	inline void process_input(GLFWwindow* _window)
	{
		// BASIC ENGINE INPUT
		glfwSetWindowShouldClose(_window, glfwGetKey(m_window, GLFW_KEY_ESCAPE));
	}

	inline void _Destroy()
	{
		Editor_Close();
		glfwTerminate();
	}
	inline void Editor_Close()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void _Loop();

	void _Init(int _Width = 1920, int _Height = 1080, const char* _AppName = "RaisinEngine");
};
#endif
