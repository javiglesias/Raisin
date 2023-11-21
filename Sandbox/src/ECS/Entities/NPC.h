#ifndef C__NPC_H
#define C__NPC_H
#include "Entity.h"

class NPC : public Entity
{
public:
	NPC();
	virtual void Init(glm::vec2 _init_position, float _width = 0.025f, float _height = 0.025f);
	~NPC();
	void Draw() override;
	Shader* m_shader;
	float FRAMECAP30;
	float FRAMECAP24;
private:
	unsigned int model_id = -1;
	unsigned int base_color_id = -1;
	unsigned int VBO = -1, VBO_Circle = -1;
	unsigned int VAO = -1, VAO_Circle = -1;
	float m_width = 0.1f;
	float m_height = 0.1f;
	Shape_Size* m_shape_size;
	/*PhysicsComponent* m_rigidbody{};
	SoundComponent* m_ears;*/
	glm::mat4 m_model = glm::mat4(1.0f);
};

#endif
