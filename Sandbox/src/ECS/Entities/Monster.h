#pragma once
#include "Entity.h"
#include "../Components/AIComponent.h"

class Monster : public Entity
{
public:
	Monster();
	~Monster();
	Monster(glm::vec3, uint8_t _health = 1);
	// Tipos de enemigos que pueden spawnear a otros.
	void Draw() override;
	void DrawEffectArea() override;
	void ResetMonster();
	virtual Monster* Clone() const
	{
		return new Monster(m_transform->m_position);
	}
private:
	unsigned int VBO = -1, VBO_Circle = -1;
	unsigned int VAO = -1, VAO_Circle = -1;
	float m_radius = 0.1f;
	int m_tex_width, m_tex_height, m_tex_channels;
	unsigned int m_texture = -1;
	unsigned char* m_tex_data;
	unsigned int model_id = -1;
	unsigned int base_color_id = -1;
	unsigned int alive_id = -1;
	uint8_t m_Health;
	Shader* m_shader;
	float m_active_distance = 0.1f;
	float m_shape[15] = {
		//	x,y,z
		0.0f, 0.0f, 0.0f,
		-0.03f, -0.06f, 0.0f,
		0.03f, -0.06f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float m_effect_area[15] = {
		//	x,y,z
		0.01f, 0.00f, 0.0f,
		-0.03f, -0.06f, 0.0f,
		0.03f, -0.06f, 0.0f,
		0.01f, 0.0f, 0.0f
	};
	AIComponent* m_brain;
};

