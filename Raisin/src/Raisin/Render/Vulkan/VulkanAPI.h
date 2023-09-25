#pragma once
// GLAD goes first
#include "glad/glad.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cstring>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include "glm.hpp"
#include "Material.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0);
	glm::vec2 texcoord = glm::vec2(0);
};

inline const std::vector<const char*> g_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

inline bool checkValidationLayerSupport()
{
	unsigned int layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	for (const char* layerName : g_ValidationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				return true;
			}
		}
	}
	return false;
}

inline GLFWwindow& _CreateWindow(const char* _name, int _Width, int _Height)
{
	GLFWwindow* m_window = nullptr;
	uint32_t m_extensionCount = 0;
	const char** m_Extensions;
	glm::mat4 m_matrix;
	glm::vec4 m_vec;

	//VULKAN THINGS
	if (!checkValidationLayerSupport()) return *m_window;
	VkInstance m_instance;
	VkApplicationInfo m_AppInfo = {};
	m_AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_AppInfo.pApplicationName = "Hello, Sailor";
	m_AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_AppInfo.pEngineName = "Raisin";
	m_AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_AppInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo m_CreateInfo = {};
	m_CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_CreateInfo.pApplicationInfo = &m_AppInfo;

	m_Extensions = glfwGetRequiredInstanceExtensions(&m_extensionCount);

	m_CreateInfo.enabledExtensionCount = m_extensionCount;
	m_CreateInfo.ppEnabledExtensionNames = m_Extensions;
	m_CreateInfo.enabledLayerCount = g_ValidationLayers.size();
	m_CreateInfo.ppEnabledLayerNames = g_ValidationLayers.data();
	VkResult m_result = vkCreateInstance(&m_CreateInfo, nullptr, &m_instance);
	if (m_result != VK_SUCCESS)
	{
		printf("ERROR CREATING INSTANCE VULKAN");
		return *m_window;
	}

	// NORMAL RENDER THINGS
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(_Width, _Height, _name, nullptr, nullptr);
	vkEnumerateInstanceExtensionProperties(nullptr, &m_extensionCount, nullptr);
	printf("Extension supported: %d", m_extensionCount);

	auto test = m_matrix * m_vec;
	return *m_window;
}
// TODO IMPLEMENT
inline unsigned int _CreateShader(const char* _v_shader_code, const char* _f_shader_code)
{
	return -1;
}

inline unsigned int _CreateTexture(unsigned char* _texture_data, int _width, int _heigth)
{
	return -1;
}

inline unsigned int _CreateBuffers(std::vector<unsigned int> _indices, std::vector<Vertex> _vertices)
{
	return -1;
}

inline void _ClearColorBuffer()
{
	return;
}

inline unsigned int _Create2DSphere(float _Radio, float _CenterX, float _CenterY, float _Angle)
{
	return -1;
}
inline void _UseShader(unsigned int _shaderId)
{
}
inline void _DrawArrays(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition,
	unsigned int _shader, unsigned int _VAO, unsigned int _primitive, unsigned int _first, int _indicesSize)
{
	_UseShader(_shader);
	return;
}
inline void _DrawElements(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition,
	glm::vec3 _LightPosition, Material* _Material, unsigned int _VAO, unsigned int _primitive, int _indicesSize)
{
	_UseShader(_Material->mShaderId);
}