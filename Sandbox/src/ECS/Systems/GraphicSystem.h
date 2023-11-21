#pragma once
#include "../Components/GraphicsComponent.h"

struct GraphicsSystem {
	GraphicsComponent m_graphics[256];
public:
	void Update();
};

