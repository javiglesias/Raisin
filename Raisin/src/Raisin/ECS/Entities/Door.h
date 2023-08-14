#pragma once
#include "Entity.h"
class Door : public Entity
{
public:
	Door() {};
	Door(glm::vec3 _init_position, float _width=0.05f, float _heigth = 0.05f);
	void Draw() override;

private:
	float m_shape[15] = {
		 0.05f, -0.05f, 0.0f,
		-0.05f, -0.05f, 0.0f,
		-0.05f,  0.05f, 0.0f,
		 0.05f,  0.05f, 0.0f
	};
	unsigned int VBO = -1;
	unsigned int VAO = -1;
	unsigned int m_model_id = -1;
	unsigned int m_base_color_id = -1;
	glm::mat4 m_model{1.0f};
	Shader* m_shader;
	//PhysicsComponent* m_rigidbody{};
};

