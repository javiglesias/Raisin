#include "PhysicsSystem.h"

void PhysicsSystem::UpdatePhysics()
{
	g_hero_physics_component->UpdatePhysics();
	for (int i = 0; i < g_total_physics_component; ++i)
	{
		g_physics_components[i]->UpdatePhysics();
		if (g_physics_components[i]->IsCollingWith(g_hero_physics_component))
			g_physics_components[i]->m_isColliding = true;
		else
			g_physics_components[i]->m_isColliding = false;

		if (g_physics_components[i]->IsCollingWith(g_escape_physics_component))
			g_physics_components[i]->m_hasEscape = true;
	}
}

void PhysicsSystem::AddStaticWorld(Entity* _entity)
{
	m_static_world.push_back(_entity);
}

void PhysicsSystem::AddDynamicWorld(Entity* _entity)
{
	m_dynamic_world.push_back(_entity);
}
void PhysicsSystem::UpdateStaticWorld(std::vector<Entity*> _static_world)
{
	m_static_world =_static_world;
}
bool PhysicsSystem::GonnaCollideWith(Entity* _entity)
{
	for (auto ent : m_static_world)
	{
		if (GonnaCollide(_entity, ent))
		{
			return true;
		}
	}
	return false;
}
/// <summary>
/// 
/// </summary>
/// <param name="_one">hero</param>
/// <param name="_two">door</param>
/// <returns></returns>
bool PhysicsSystem::GonnaCollide(Entity* _one, Entity* _two)
{
	//if (_one->GetNextPosition().x <= (_two->GetPosition().x + _two->GetSize().w) &&
	//	_one->GetNextPosition().x >= _two->GetPosition().x &&
	//	_one->GetNextPosition().y <= (_two->GetPosition().y + _two->GetSize().h) &&
	//	_one->GetNextPosition().y >= _two->GetPosition().y)
	//{
	//	//	TODO implementar colisiones en las demas entidades, no solo el NPC.
	//	return false;
	//}
	return false;
}

void PhysicsSystem::FreeOutOfBounds(std::vector<Entity*> *_entities)
{
	/*for (auto _entity : *_entities)
	{
		if (_entity->GetPosition().x > bound_right_top ||
			_entity->GetPosition().x < bound_left_bottom ||
			_entity->GetPosition().y > bound_right_top ||
			_entity->GetPosition().y < bound_left_bottom)
		{
			if(!_entity->m_to_delete)
			{
				_entity->m_to_delete = true;
				_entity->m_show = false;
			}
		}
	}*/
}

void PhysicsSystem::IsColliding(Entity* _one, std::vector<Entity*> _entities)
{
	//	Aqui hay que recorrer todas las entidades que se le pasan y comprobar si esta colisionando con alguna de ellas.
	/*for (auto _entity : _entities)
	{
		if (_one->GetNextPosition().x > _entity->GetPosition().x + 0.1 &&
			_one->GetNextPosition().x < _entity->GetPosition().x + 0.1 &&
			_one->GetNextPosition().y > _entity->GetPosition().y + 0.1 &&
			_one->GetNextPosition().y < _entity->GetPosition().y + 0.1)
		{
			fprintf(stdout, "Colisiono con algo");
		}
	}*/
}
