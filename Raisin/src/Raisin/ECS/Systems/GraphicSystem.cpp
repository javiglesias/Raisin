#include "GraphicSystem.h"

void GraphicsSystem::Update()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_graphics[i].Update();
	}
}
