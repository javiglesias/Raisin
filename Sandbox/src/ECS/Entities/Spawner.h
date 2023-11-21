#pragma once
#include "Monster.h"

class Spawner
{
public:
	Spawner(Monster* _prototype) : m_prototype(_prototype){}
	Monster* DoSpawn()
	{
		return m_prototype->Clone();
	}
private:
	Monster* m_prototype;
};

