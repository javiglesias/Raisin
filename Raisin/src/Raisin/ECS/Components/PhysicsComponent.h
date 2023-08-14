#pragma once
#include "glm.hpp"
#include "Component.h"

class PhysicsComponent : public cComponent
{
public:
	PhysicsComponent() {}
	~PhysicsComponent(){}
	PhysicsComponent(glm::vec3 _position, float _width, float _height, bool _static = true);
	void virtual UpdatePhysics();
	void virtual UpdatePosition(glm::vec3);
	bool IsCollingWith(PhysicsComponent*);
	void Move(glm::vec3 _scrolling);
	glm::mat4 m_model = glm::mat4(1.0);
	glm::vec3 m_position = glm::vec3(0);
	bool m_isColliding = false;
	bool m_hasEscape = false;
	float m_width{0.1f}, m_height{0.1f};
	bool m_is_static = true;
};

