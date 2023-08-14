#pragma once
#include "Room.h"
class Corridor : public Room
{
public:
	Corridor(glm::vec3 _init_position, Shader* _shader,
		Color _color = Color(153.f / 255.f, 76.f / 255.f, 0.f));
	void Draw() override;
private:
	float m_shape[18] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.075f, 0.0f,
		0.1f, 0.0f, 0.0f,
		//	1er triangulo

		0.1f, 0.0f, 0.0f,
		0.0f, 0.075f, 0.0f,
		0.1f, 0.075f, 0.0f
		//	2nd triangulo
	};
	unsigned int VBO = -1;
	unsigned int VAO = -1;
	unsigned int model_id = -1;
	unsigned int base_color_id = -1;
	unsigned int alive_id = -1;
	Shape_Size* m_shape_size;
	Shader* m_shader;
	glm::mat4 m_model = glm::mat4(1.0f);
	Color m_shader_base_color;
};
