#include "NPC.h"

NPC::NPC() {}


void NPC::Init(glm::vec2 _init_position, float _width, float _height)
{
	//m_ears = new SoundComponent();
	//m_ears->PlaySFX(m_ears->WALK);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	model_id = glGetUniformLocation(m_shader->id, "model");
	base_color_id = glGetUniformLocation(m_shader->id, "base_color");
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_shape), m_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	/*g_physics_components[g_total_physics_component] = new PhysicsComponent(_init_position, 0.1f, 0.1f, false);
	m_transform = g_physics_components[g_total_physics_component];
	g_total_physics_component++;*/
	m_transform = glm::vec3(_init_position.x * _width,
		_init_position.y * _height, 1.f);
	m_state = READY;
}

NPC::~NPC()
{
	printf("~NPC: %p\n", this);
}

inline void NPC::Draw()
{
	if (m_state == INVALID) return;
	m_model = glm::mat4(1.f);
	m_transform += g_scrolling * g_DefaultMovFactor;
	m_model = translate(m_model, m_transform);
	if (m_transform.x > .99f || m_transform.x < -.99f
		|| m_transform.y > .99f || m_transform.y < -.99f)
		return;
	glBindVertexArray(VAO);
	glUniform4f(base_color_id, 0, 1, 0, 1);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	m_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 4);
	m_shader->unuse();
	glBindVertexArray(0);
}

//
//bool NPC::UpdatePhysics(Entity* _target, bool _interacting, void (*_callback) ())
//{
//	if (_target->GetPosition().x < (Entity::m_position.x + m_shape_size->w) &&
//		_target->GetPosition().x >= Entity::m_position.x &&
//		_target->GetPosition().y < (Entity::m_position.y + m_shape_size->h) &&
//		_target->GetPosition().y >= Entity::m_position.y)
//	{
//		// llamada de callback cuando ha hablado con el NPC
//		(*_callback)();
//		return true;
//	}
//	if(_interacting)
//	{
//		fprintf(stdout, "NPC: (%.3f,%.3f)\n", Entity::m_position.x, Entity::m_position.y);
//		fprintf(stdout, "NPC: (%.3fx%.3f)\n", Entity::m_position.x + m_shape_size->w,
//			Entity::m_position.y + m_shape_size->h);
//	}
//	return false;
//}

