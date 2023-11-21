#include "Hero.h"

void Hero::Init(glm::vec2 _init_position, float _width, float _height)
{
	//PROFILING::_start_(PROFILING::PROF_TYPE::LOGIC, "INIT_HERO");
	//isHero = true;

	//int w, h, nChannels;
	shader = new Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag", "HERO");
	/*strcpy(m_texturePath, "resources/art/grass.jpg");
	unsigned char* data = stbi_load(m_texturePath, &w, &h, &nChannels, 0);*/
	m_width = _width;
	m_height = _height;
	model_id = glGetUniformLocation(shader->id, "model");
	base_color_id = glGetUniformLocation(shader->id, "base_color");

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	/*if(data)
	{
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture_id);
		stbi_image_free(data);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}*/

	// DEBUG BUFFERS
	/*glGenBuffers(1, &debug_VBO);
	glGenVertexArrays(1, &debug_VAO);
	glBindVertexArray(debug_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, debug_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_collisionShape), m_collisionShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);*/

	m_transform = glm::vec3(0);
	m_model = translate(m_model, m_transform);
	//m_debugModel = translate(m_debugModel, glm::vec3(-.05f, -.075f, 1.f));
	//PROFILING::_end_(PROFILING::PROF_TYPE::LOGIC, "INIT_HERO");
}

void Hero::Draw()
{
	if (model_id < 0) return;
	unsigned int iTime_uniform = glGetUniformLocation(shader->id, "iTime");
	if( iTime_uniform >= 0)
		glUniform1f(iTime_uniform, iTime);
	// DEBUG DRAW
	/*glBindVertexArray(debug_VAO);
	shader->use();
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_debugModel));
	glUniform4f(base_color_id, 1, 0, 0, 1);
	glDrawArrays(GL_LINES, 0, 8);
	glBindVertexArray(0);*/
	// MODEL DRAW
	glBindVertexArray(VAO);
	m_model = glm::mat4(1.f);
	m_model = translate(m_model, m_transform);
	//glBindTexture(GL_TEXTURE_2D, texture_id);
	shader->use();
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	glDrawArrays(GL_TRIANGLES, 0, 4);
	//glBindTexture(GL_TEXTURE_2D, 0);
	shader->unuse();
	glBindVertexArray(0);
}

void Hero::DebugDraw()
{
	Entity::DebugDraw();

}

void Hero::Tick(float _timediff)
{
	Entity::Tick(_timediff);
	iTime += _timediff;
	m_oldTransform = m_transform;
	m_transform = m_oldTransform + m_velocity + m_acceleration * _timediff * _timediff;
	// Simulamos la siguiente posicion, si esta fuera de los limites, hacemos fallback
	if (m_transform.y >= (1.f) || m_transform.y <= ( - 1.f))
		m_transform = m_oldTransform;
}
