#pragma once
#include "Component.h"
class GraphicsComponent : public cComponent
{
public:
	GraphicsComponent();
	~GraphicsComponent();
	void Update() override;
};

