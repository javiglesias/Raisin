#include "Corridor.h"

Corridor::Corridor(glm::vec3 _init_position, Shader* _shader, Color _color)
{
	m_shader = _shader;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	model_id = glGetUniformLocation(m_shader->id, "model");
	base_color_id = glGetUniformLocation(m_shader->id, "base_color");
	alive_id = glGetUniformLocation(m_shader->id, "is_alive");
	//	Posicion de inicio donde va a empezar el enemigo
	
	m_shape_size = new Shape_Size(0.1f, 0.1f);
	m_shader_base_color = _color;

	g_physics_components[g_total_physics_component] = new PhysicsComponent(_init_position, 0.1f, 0.1f, false);
	m_transform = g_physics_components[g_total_physics_component];
	g_total_physics_component++;
}

void Corridor::Draw()
{
	glBindVertexArray(VAO);
	glUniform4f(base_color_id, m_shader_base_color.r, m_shader_base_color.g, 
		m_shader_base_color.b, m_shader_base_color.a);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	m_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unuse();
}
