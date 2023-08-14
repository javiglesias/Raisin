#pragma once
#include "../Systems/SoundSystem.h"
#include "Component.h"
class SoundComponent : public cComponent
{
public:
	enum SFX_Type
	{
		WALK,
		ATTACK,
		DIE
	};
	SoundComponent();
	void PlaySFX(SFX_Type);
private:
	SoundSystem* m_sound_system;
};

