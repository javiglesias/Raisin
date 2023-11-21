#include "SoundSystem.h"
#include <cstring>

SoundSystem::SoundSystem()
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
		fprintf(stdout, "FMOD init NOK.\n");
}

SoundSystem::~SoundSystem()
{
	m_sound_effect->release();
	m_music_sound->release();
	m_sound_system->close();
	m_music_system->close();
}

void SoundSystem::Update()
{
	m_music_system->getCPUUsage(&usage);
	FMOD_ADVANCEDSETTINGS settings;
	m_music_system->getAdvancedSettings(&settings);
}

void SoundSystem::PlaySFX(const char* _sound_name)
{
	//	Long sound playing
	char path[256] = "";
	strcat(path, m_sfx_path);
	strcat(path, _sound_name);
	fmod_result = m_sound_system->createSound(path, FMOD_DEFAULT, 0, &m_sound_effect);
	FMOD_ERRCHECK(fmod_result);
	fmod_result = m_sound_system->playSound(m_sound_effect, 0, false, &m_channel_sfx);
	m_channel_sfx->setVolume(m_volume);
	FMOD_ERRCHECK(fmod_result);
}

void SoundSystem::StopSFX()
{
	m_sound_effect->release();
}
void SoundSystem::PlayMusic(const char* _music_name, float _speed)
{
	//	Long sound playing
	char path[256] = "";
	strcat(path, m_music_path);
	strcat(path, _music_name);
	/*fmod_result = m_music_system->createSound(path, FMOD_DEFAULT, 0, &m_music_sound);
	m_music_sound->setMode(FMOD_CREATESAMPLE);
	m_music_sound->setMusicSpeed(_speed);
	FMOD_ERRCHECK(fmod_result);
	fmod_result = m_music_system->playSound(m_music_sound, 0, false, &m_channel_music);
	m_channel_music->setVolume(m_volume);
	FMOD_ERRCHECK(fmod_result);*/

	FMOD_CREATESOUNDEXINFO   exinfo2;
	memset(&exinfo2, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo2.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo2.decodebuffersize = 44100;
	exinfo2.length = 44100 * 1 * sizeof(float) * 100;
	exinfo2.numchannels = 1;
	exinfo2.defaultfrequency = 44100;
	exinfo2.format = FMOD_SOUND_FORMAT_PCMFLOAT;
	fmod_result = m_music_system->createStream(path, FMOD_LOOP_NORMAL | FMOD_CREATESTREAM, &exinfo2, &m_music_sound);
	FMOD_ERRCHECK(fmod_result)
	fmod_result = m_music_system->playSound(m_music_sound, m_channel_group, false, 0);
}
void SoundSystem::StopMusic()
{
	m_music_sound->release();
}

void SoundSystem::StopAll()
{
	m_sound_effect->release();
	m_music_sound->release();
}

void SoundSystem::VolumeIncrement(float value)
{
	if((m_volume + value) < 0.0f)
	{
		m_volume = 0.0f;
	}
	else if((m_volume + value) > 1.0f)
	{
		m_volume = 1.0f;
	}
	else
	{
		m_volume += value;
	}
	m_channel_music->setVolume(m_volume);
	m_channel_sfx->setVolume(m_volume);
}

float SoundSystem::GetSample()
{
	return usage.stream;
}

