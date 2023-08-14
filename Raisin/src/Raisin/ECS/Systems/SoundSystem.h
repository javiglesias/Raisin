#pragma once
#include "fmod.hpp"
#include <cstdio>
#define FMOD_ERRCHECK(X) if(X != FMOD_OK) fprintf(stdout, "ERROR ON FMOD %d", X);
class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void Update();
	void PlaySFX(const char*);
	void StopSFX();
	void PlayMusic(const char*, float _speed = 1.f);
	void StopMusic();
	void StopAll();
	void VolumeIncrement(float value);
	float GetSample();
private:
	const char* m_sfx_path = "resources\\SFX\\";
	const char* m_music_path = "resources\\Music\\";
	float m_volume = .5f;
	FMOD_RESULT fmod_result;
	FMOD::System* m_sound_system = nullptr, *m_music_system = nullptr;
	FMOD::Sound* m_sound_effect, *m_music_sound;
	FMOD::Channel* m_channel_sfx = 0, *m_channel_music = 0;
	FMOD::ChannelGroup* m_channel_group;
	FMOD_CPU_USAGE usage;
};

