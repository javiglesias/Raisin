#include "PersistenceUtils.h"
#include "../ECS/Entities/Hero.h"
bool PersistenceUtils::SaveObject(const Entity& _ent)
{
    fprintf(stdout, "%s", _ent.GetPersistData().c_str());
	//std::string containerName = "resources/containers/" + _ent.GetName() + ".json";
	//FILE* container = fopen(containerName.c_str(), "w");
	/*if (container)
	{
		fprintf(container, "%s", _ent.GetPersistData().c_str());
		fclose(container);
		fprintf(stdout, "container saved.");
	}*/
    return false;
}

void PersistenceUtils::onNotify(const Entity& _entity, Event _event)
{
	bool isHero = _entity.isHero;
	switch (_event.m_type)
	{
	case SAVE_ENTITY:
		SaveObject(_entity);
		break;
	case DEATH:
		if (isHero)
		{

		}
		break;
	case REVIVE:
		if (isHero)
		{

		}
		break;
	default:
		break;
	}
}
