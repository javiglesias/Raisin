#ifndef C__HERO_H
#define C__HERO_H

#include "Entity.h"

class Hero : public Entity
{
public:
	Hero() = default;
    virtual void Init(glm::vec2 _position, float _width = .05f, float _height = .05f);
	void Draw() override;
	void DebugDraw() override;
	void virtual Tick(float _timediff) override;

private:
	Shader* shader;
	float iTime = 0;
	unsigned int m_width, m_height;
	glm::mat4 m_model{ 1.0f };
	glm::mat4 m_debugModel{ 1.0f };
	int model_id = -1;
	int base_color_id = -1;
	int texture_id = -1;
	GLuint VBO = -1;
	GLuint VAO = -1;
	GLuint debug_VBO = -1;
	GLuint debug_VAO = -1;
	char m_texturePath[MAX_STRING_LENGTH];
	glm::vec3 m_oldTransform {0.f};
	glm::vec3 m_velocity{ 0, -0.0098f, 0 };
	glm::vec3 m_acceleration{ 0, -0.0098f, 0 };
};
#endif
