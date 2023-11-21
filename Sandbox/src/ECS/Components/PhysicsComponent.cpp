#include "PhysicsComponent.h"
#include "gtc/matrix_transform.hpp"

PhysicsComponent::PhysicsComponent(glm::vec3 _position, float _width, float _height, bool _static)
{
	m_position = _position;
	m_width = _width;
	m_height = _height;
	m_is_static = _static;
	m_model = glm::translate(glm::mat4(1.f), m_position);
}

void PhysicsComponent::UpdatePosition(glm::vec3 _position)
{
	m_position += _position;
	m_model = glm::translate(m_model, glm::vec3(_position.x, _position.y, 0.0f));
}

bool PhysicsComponent::IsCollingWith(PhysicsComponent* _positionOther)
{
	bool isXCoordColl  = (m_position.x - m_width / 2) < _positionOther->m_position.x; // x
	bool isYCoordColl  = m_position.y > _positionOther->m_position.y; // y
	bool isXWCoordColl = (m_position.x + m_width/2) > _positionOther->m_position.x; // x + W
	bool isYHCoordColl = (m_position.y - m_height) < _positionOther->m_position.y; // y - H
	if (isXCoordColl && isYCoordColl && isXWCoordColl && isYHCoordColl)
	{
		return true;
	}
	return false;
}

void PhysicsComponent::Move(glm::vec3 _scrolling)
{
	if (m_position.y > -.9)
		_scrolling.y = 0.00098f;
	m_position -= _scrolling;
	m_model = glm::translate(m_model, glm::vec3(-_scrolling.x, -_scrolling.y, 0.0f));
}

void PhysicsComponent::UpdatePhysics()
{
	if(!m_is_static)
		m_model = glm::translate(glm::mat4(1.f), m_position);
}
