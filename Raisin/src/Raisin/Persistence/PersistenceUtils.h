#pragma once
#include "../Observers/Observer.h"
#include "../Observers/Event.h"
#include "../ECS/Entities/Entity.h"

#define REFLECT(x) 

class PersistenceUtils : public Observer
{
public:
	PersistenceUtils(){ }
	bool SaveObject(const Entity& _ent);
	virtual void onNotify(const Entity& _entity, Event event) override;
};

