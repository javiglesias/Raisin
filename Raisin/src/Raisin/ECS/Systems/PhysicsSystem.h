// ReSharper disable CppInconsistentNaming
#pragma once
#include <vector>
#include "../Entities/Entity.h"
class PhysicsSystem
{
public:
	PhysicsSystem() = default;
	void UpdatePhysics();
	void AddStaticWorld(Entity*);
	void AddDynamicWorld(Entity*);
	bool GonnaCollideWith(Entity*);
	static bool GonnaCollide(Entity* _one, Entity* _two);
	void FreeOutOfBounds(std::vector<Entity*>*);
	static void IsColliding(Entity* _entity, std::vector<Entity*> _collisionable_world);
	void UpdateStaticWorld(std::vector<Entity*> _static_world);
private:
	std::vector<Entity*> m_static_world{};	//	Colisiones estaticas
	std::vector<Entity*> m_dynamic_world{};
	float bound_left_bottom = -0.99f;
	float bound_right_top = 0.99f;
};

