#pragma once

// Sound manager
#ifdef _WINDOWS
#include <fmod.hpp>
#include "../DebugUtils/Debug.h"
#define FMOD_ERRCHECK(X) if(X != FMOD_OK) fprintf(stdout, "ERROR ON FMOD %d", X)

float m_volumeSFX = 1.f, m_volumeMUSIC = 1.f;
float m_soundWave[12];
FMOD_RESULT fmod_result;
FMOD::System* m_sound_system = nullptr, * m_music_system = nullptr;
FMOD::Sound* m_sound_effect, * m_music_sound;
FMOD::Channel* m_channel_sfx = 0, * m_channel_music = 0;
FMOD::ChannelGroup* m_channel_group;
FMOD_CPU_USAGE usage;

namespace RaisinEng
{
	inline void InitSoundSystem()
	{
		//	FMOD init
		fmod_result = FMOD::System_Create(&m_sound_system);
		FMOD_ERRCHECK(fmod_result);
		fmod_result = FMOD::System_Create(&m_music_system);
		FMOD_ERRCHECK(fmod_result);
		fmod_result = m_sound_system->init(32, FMOD_INIT_NORMAL, 0);
		FMOD_ERRCHECK(fmod_result);
		fmod_result = m_music_system->init(32, FMOD_INIT_NORMAL, 0);
		FMOD_ERRCHECK(fmod_result);
		if (fmod_result != FMOD_OK)
			INFO_LOG("FMOD init NOK.\n")
		else
			INFO_LOG("FMOD init OK.\n")
	}
	inline void DestroySoundSystem()
	{
		m_sound_effect->release();
		m_music_sound->release();
		m_sound_system->close();
		m_music_system->close();
	}
	inline void PlaySFX(const char* _soundFile)
	{
		char path[256] = "";
		strcat(path, _soundFile);
		fmod_result = m_sound_system->createSound(path, FMOD_2D, 0, &m_sound_effect);
		FMOD_ERRCHECK(fmod_result);
		fmod_result = m_sound_system->playSound(m_sound_effect, 0, false, &m_channel_sfx);
		m_channel_sfx->setVolume(m_volumeSFX);
		FMOD_ERRCHECK(fmod_result);
	}
	inline void SoundSysTick(float _deltaTime)
	{
		m_channel_sfx->setVolume(m_volumeSFX);
		m_channel_music->setVolume(m_volumeMUSIC);
		//int channelsPlaying = -1;
		//if(m_sound_system->getChannelsPlaying(&channelsPlaying) == FMOD_OK)
		//{
		//	for (int i = 0; i < channelsPlaying; i++)
		//	{
		//		FMOD::Channel* channel;
		//		if(m_sound_system->getChannel(m_channel_sfx, &channel) == FMOD_OK)
		//		{
		FMOD::Sound* currentSound;

		if(m_channel_sfx->getCurrentSound(&currentSound) == FMOD_OK)
		{
			char name[64] = "";
			int nameLen = -1;
			currentSound->getName(name, nameLen);
			if(nameLen > 0)
				INFO_LOG(name)
		}
		else
		{
			// TODO RELEASE THE CHANNEL
		}
		//		}
		//	}
		//}
	}
	float* GetSetVolumeSFX()
	{
		return &m_volumeSFX;
	}
	float* GetSetVolumeMUSIC()
	{
		return &m_volumeMUSIC;
	}
	float* GetSoundWave()
	{
		m_soundWave[0] = m_volumeSFX;
		m_soundWave[1] = m_volumeMUSIC;
		return m_soundWave;
	}
}
#endif
