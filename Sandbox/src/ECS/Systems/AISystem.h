#pragma once
#include <vector>
#include "../Entities/Entity.h"
#include "../Components/AIComponent.h"

class AISystem
{
	AIComponent* m_ai[256];
public:
	inline AISystem() {
		for (unsigned int i = 0; i < 256; i++)
		{
			m_ai[i] = new AIComponent();
		}
	}
	void Update(Entity* _hero, std::vector<Entity*> _friendos);
	void Update();
};

