#pragma once
#include "glm.hpp"

class PhysicsComponent;

class AIComponent
{
public:
	AIComponent() {}
	glm::vec2 virtual Update(glm::vec2 _target_position,
		glm::vec2 _current_position);
	void Update(PhysicsComponent& _me);
};
