#pragma once
// GLAD goes first
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <iostream>
#include <vector>

struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0);
	glm::vec2 texcoord = glm::vec2(0);
};

inline GLFWwindow& _CreateWindow(const char* _name, int _Width, int _Height)
{
	GLFWwindow* m_window = nullptr;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(_Width, _Height, _name,
#ifdef _GAME
		glfwGetPrimaryMonitor(),
#else
		nullptr,
#endif
		nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		exit(-1);
	}
	//	GLAD
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		exit(-1);
	}
	glViewport(0, 0, _Width, _Height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return *m_window;
}

inline unsigned int _CreateShader(const char* _v_shader_code, const char* _f_shader_code)
{
	unsigned int id;
	unsigned int vertex, fragment;
	int success;
	char log_info[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &_v_shader_code, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &_f_shader_code, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return id;
}

inline unsigned int _CreateTexture(unsigned char* _texture_data, int _width, int _heigth)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _heigth, 0, 
		GL_RGB, GL_UNSIGNED_BYTE, _texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

inline unsigned int _CreateBuffers(std::vector<unsigned int> _indices, std::vector<Vertex> _vertices)
{
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (_vertices.size() <= 0) __debugbreak();
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
		&_vertices[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
		&_indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, texcoord));
	glBindVertexArray(0);
	return VAO;
}

inline void _ClearColorBuffer()
{
	glClearColor(0, 1.f, 0, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

inline unsigned int _Create2DSphere(float _Radio, float _CenterX, float _CenterY, float _Angle)
{
	unsigned int VAO, VBO;
	float mVertices[48];
	for (int i = 0; i < 48; i += 3)
	{
		mVertices[i] = (_CenterX + _Radio * cos(i * _Angle)) / 800.f;// x = cx + r * cos(a)
		mVertices[i + 1] = (_CenterY + _Radio * sin(i * _Angle)) / 600.f; //y = cy + r * sin(a)
		mVertices[i + 2] = 0.f;
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	return VAO;
}

inline void _UseShader(unsigned int _shaderId)
{
	glUseProgram(_shaderId);
}
inline void _DrawArrays(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition,
	unsigned int _shader, unsigned int _VAO, unsigned int _primitive,unsigned int _first, int _indicesSize)
{
	// obtenemos los uniforms para las transformaciones 3d
	unsigned int modelMatrix_location = glGetUniformLocation(_shader, "ModelMatrix");
	unsigned int viewMatrix_location = glGetUniformLocation(_shader, "ViewMatrix");
	unsigned int ProjectionMatrix_location = glGetUniformLocation(_shader, "ProjectionMatrix");
	unsigned int cameraPosition_location = glGetUniformLocation(_shader, "CameraPosition");

	_UseShader(_shader);
	glBindVertexArray(_VAO);
	// Mandamos los uniforms a la GPU
	if (modelMatrix_location != -1)
		glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, value_ptr(_ModelMatrix));
	if (cameraPosition_location != -1)
		glUniform3fv(cameraPosition_location, 1, value_ptr(_CameraPosition));
	if (viewMatrix_location != -1)
		glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, value_ptr(_ViewMatrix));
	glm::mat4 projection = glm::perspective(glm::radians(90.f), 1.f,
		0.3f, 10.f);
	glUniformMatrix4fv(ProjectionMatrix_location, 1, GL_FALSE, value_ptr(projection));
	glDrawArrays(_primitive, _first, _indicesSize);
}

inline void _DrawElements(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition,
	unsigned int _shader, unsigned int _VAO, unsigned int _primitive, int _indicesSize)
{
	// obtenemos los uniforms para las transformaciones 3d
	unsigned int modelMatrix_location = glGetUniformLocation(_shader, "ModelMatrix");
	unsigned int viewMatrix_location = glGetUniformLocation(_shader, "ViewMatrix");
	unsigned int projectionMatrix_location = glGetUniformLocation(_shader, "ProjectionMatrix");
	unsigned int cameraPosition_location = glGetUniformLocation(_shader, "CameraPosition");

	_UseShader(_shader);
	glBindVertexArray(_VAO);
	// Mandamos los uniforms a la GPU
	if (modelMatrix_location != -1)
		glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, value_ptr(_ModelMatrix));
	if (cameraPosition_location != -1)
		glUniform3fv(cameraPosition_location, 1, value_ptr(_CameraPosition));
	if (viewMatrix_location != -1)
		glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, value_ptr(_ViewMatrix));
	if (projectionMatrix_location != -1)
		glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, value_ptr(_ProjectionMatrix));
	glDrawElements(_primitive, _indicesSize, GL_UNSIGNED_INT, 0);
}
