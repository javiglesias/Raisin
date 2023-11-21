#pragma once
#include "Entity.h"
class Room : public Entity
{
public:
	enum TYPE
	{
		NONE,
		WATER,
		HALL,
		CORRIDOR
	};
public:
	Room() { m_state = INVALID; }
	virtual void Init( glm::vec2 _position, float _width = 0.1f, float _height = 0.1f);
	~Room();
	//PhysicsComponent* GetPhysicsComponent() override;
	void SetShaderColor(Color _color);
	void virtual Draw() override;
	void virtual Tick(float _timediff) override;
	TYPE m_type { NONE };
	Color m_shader_base_color = Color(153.f / 255.f, 76.f / 255.f, 0.f);
private:
	float m_shape[18] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.1f, 0.0f, 0.0f,
		//	1er triangulo

		0.1f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.1f, 0.1f, 0.0f
		//	2nd triangulo
	};
	unsigned int VBO = -1;
	unsigned int VAO = -1;
	int base_color_id = -1;
	int model_id = -1;
	int texture_id = -1;
	char m_texturePath[MAX_STRING_LENGTH];
	glm::mat4 m_model{1.0f};
	float iTime = 0;
	Shape_Size m_shape_size;
	Shader* m_shader;
};	

