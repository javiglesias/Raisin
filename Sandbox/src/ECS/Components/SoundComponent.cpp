#include "SoundComponent.h"
#include <cstring>
#include <random>
#include <cstdio>

SoundComponent::SoundComponent()
{
	//m_sound_system = new SoundSystem();
}

void SoundComponent::PlaySFX(SFX_Type _sfx)
{
	char death_path[64] = "";
	int rng = rand() % 2;
	switch (_sfx)
	{
	case SFX_Type::WALK:
		//m_sound_system->PlaySFX("walk.wav");
		break;
	case SFX_Type::ATTACK:
		//m_sound_system->PlaySFX("gasp.wav");
		break;
	case SFX_Type::DIE:
		sprintf(death_path, "death%d.wav", rng);
		//m_sound_system->PlaySFX(death_path);
		break;
	default:
		break;
	}
}
