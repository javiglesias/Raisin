#include "Door.h"

Door::Door(glm::vec3 _init_position, float _width, float _heigth)
{
	m_shader = new Shader("code\\shaders\\basic.vert", "code\\shaders\\basic.frag", "DOOR");
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	m_model_id = glGetUniformLocation(m_shader->id, "model");
	m_base_color_id = glGetUniformLocation(m_shader->id, "base_color");

	g_escape_physics_component = new PhysicsComponent(_init_position, 0.05f, 0.05f, true);
	m_transform = g_escape_physics_component;
}

void Door::Draw()
{
	glBindVertexArray(VAO);
	glUniform4f(m_base_color_id, 0.4f, 0.5f, 1.f, 1.f);
	glUniformMatrix4fv(m_model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	m_shader->use();
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	m_shader->unuse();
}