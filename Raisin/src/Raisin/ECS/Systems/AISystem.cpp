#include "AISystem.h"

void AISystem::Update(Entity* _hero, std::vector<Entity*> _friendos)
{
	for (auto entity : _friendos )
	{
		//entity->UpdateIA(_hero);
	}
}

void AISystem::Update() {
	for (uint8_t i = 0; i < g_total_ai_component; i++)
	{
		g_ai_components[i]->Update(*g_physics_components[i]);
	}
}
