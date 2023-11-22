#pragma once
#include "Material.h"
#include <iostream>
#include <vector>
// GLAD goes first
#include <glad/glad.h>
#include "glm.hpp"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../stb_image/stb_image.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_node_editor.h"
#include "../imgui/ImFileDialog.h"


struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0);
	glm::vec2 texcoord = glm::vec2(0);
};

inline GLFWwindow& _CreateWindow(const char* _name, int _Width, int _Height)
{
	GLFWwindow* m_window = nullptr;
	glfwInit();
	glfwSetErrorCallback([](int _code, const char* _desc)
		{
			fprintf(stderr, "GLFW Error(%d): %s\n", _code, _desc);
		});
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

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
		std::cerr << "VERT_SHADER_ERROR: " << log_info;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &_f_shader_code, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, log_info);
		std::cerr << "FRAG_SHADER_ERROR: " <<log_info;
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

inline const unsigned int _CreateTextureFromFile(const char* _str)
{
	int width, heigth, nr_channels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* texture_data = stbi_load(_str,
		&width, &heigth, &nr_channels, 0);
	unsigned int texture = -1;
	if (texture_data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texture_data);
	}
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
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
		&_vertices[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
		&_indices[0], GL_DYNAMIC_DRAW);
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)0);
	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, normal));

	// Texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, texcoord));
	glBindVertexArray(0);
	return VAO;
}

inline void _ClearColorBuffer()
{
	glClearColor(0, 188.f/255.f, 255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

inline unsigned int _CreateVAOData(float* _Vertices, int _VertSize)
{
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _VertSize, _Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	return VAO;
}

inline void _SetShadeUniformrBool(unsigned int _shaderId, const char* _name, bool value) 
{
	glUniform1i(glGetUniformLocation(_shaderId, _name), (int)value);
}

inline void _SetShadeUniformFloat(unsigned int _shaderId, const char* _name, float value)
{
	glUniform1f(glGetUniformLocation(_shaderId, _name), value);
}

inline void _SetShadeUniformInt(unsigned int _shaderId, const char* _name, int value)
{
	glUniform1i(glGetUniformLocation(_shaderId, _name), value);
}

inline void _SetShadeUniformVec3(unsigned int _shaderId, const char* _name, glm::vec3 value)
{
	//glUniform3fv(glGetUniformLocation(id, name.c_str()), value);
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

inline unsigned int _ShaderUsed = -1;
inline void _DrawElements(glm::mat4 _ModelMatrix, glm::mat4 _ViewMatrix, glm::mat4 _ProjectionMatrix, glm::vec3 _CameraPosition,
	Material* _Material, unsigned int _VAO, unsigned int _primitive, int _indicesSize)
{
	// obtenemos los uniforms para las transformaciones 3d
	unsigned int modelMatrix_location = glGetUniformLocation(_Material->mShaderId, "ModelMatrix");
	unsigned int viewMatrix_location = glGetUniformLocation(_Material->mShaderId, "ViewMatrix");
	unsigned int projectionMatrix_location = glGetUniformLocation(_Material->mShaderId, "ProjectionMatrix");
	unsigned int cameraPosition_location = glGetUniformLocation(_Material->mShaderId, "ViewerPosition");
	unsigned int lightPosition_location = glGetUniformLocation(_Material->mShaderId, "LightPosition");
	unsigned int lightColor_location = glGetUniformLocation(_Material->mShaderId, "LightColor");
	// Si ya tenemos el shader en uso (optimo) entonces no volvemos a cambiar el estado.
	if(_ShaderUsed != _Material->mShaderId)
	{
		_ShaderUsed = _Material->mShaderId;
		_UseShader(_Material->mShaderId);
	}

	glPolygonMode(GL_FRONT, _Material->mMode);
	glBindVertexArray(_VAO);

	// Mandamos los uniforms a la GPU
	if (modelMatrix_location != -1)
		glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, value_ptr(_ModelMatrix));
	if (viewMatrix_location != -1)
		glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, value_ptr(_ViewMatrix));
	if (projectionMatrix_location != -1)
		glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, value_ptr(_ProjectionMatrix));
	if (cameraPosition_location != -1)
		glUniform3fv(cameraPosition_location, 1, value_ptr(_CameraPosition));
	/*if (lightPosition_location != -1)
		glUniform3fv(lightPosition_location, 1, value_ptr(_LightPosition));
	if (lightColor_location != -1)
		glUniform3fv(lightColor_location, 1, value_ptr(_LightColor));*/

	glDrawElements(_primitive, _indicesSize, GL_UNSIGNED_INT, 0);
}
