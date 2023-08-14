#include "AIComponent.h"
#include "../../common.h"
#include "PhysicsComponent.h"

glm::vec2 AIComponent::Update(glm::vec2 _target_position,
                              glm::vec2 _current_position)
{
	glm::vec2 direction_forward = glm::vec2(
		_target_position.x - _current_position.x,
		_target_position.y - _current_position.y
		);
	glm::vec2 direction_backward = glm::vec2(
		_target_position.x - _current_position.x,
		_target_position.y - _current_position.y
	);
	direction_forward = glm::normalize(direction_forward);
	direction_backward = glm::normalize(direction_backward);
	auto direction = GetRandomInt() > 100.f ? direction_backward : direction_forward;
	glm::vec2 move_to = glm::vec2(0.005f * direction.x,
		0.005f * direction.y);
	return -move_to;
}

void AIComponent::Update(PhysicsComponent& _me)
{
	if(GetRandomBoolean())
	{
		glm::vec3 escape = g_escape_physics_component->m_position;
		glm::vec3 direction = glm::vec3(
			escape.x - _me.m_position.x,
			escape.y + _me.m_width / 2 - _me.m_position.y,
			0.f
		);
		_me.m_position += direction * 0.2f;
	}
}