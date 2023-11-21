#pragma once
#include "Event.h"
#include "../ECS/Entities/Entity.h"
class Observer
{
public:
	Observer(){}
	virtual ~Observer(){}
	virtual void onNotify(const Entity& _entity, Event event) = 0;
};

