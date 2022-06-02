#include "stdafx.h"
#include "client/util/fmod/fmod.hpp"
#include "client/util/fmod/fmod_errors.h"

#include "client/asset/sound/sound.h"

namespace client_fw
{
	Sound::Sound(FMOD::System* system, UINT index, const std::string& name)
	{
		m_system = system;
		m_id = index;
		m_name = name;
		m_size = 0;
		m_pos = 0;
	}
	bool Sound::Initialize()
	{
		return true;
	}

	void Sound::Update(float delta_time)
	{
		if (m_sound != nullptr || m_channel != nullptr)
			return;
		m_sound->getLength(&m_size, FMOD_TIMEUNIT_MS);
		m_channel->getPosition(&m_pos, FMOD_TIMEUNIT_MS);
	}

	void Sound::Shutdown()
	{
		if (m_sound)
		{
			m_sound->release();
			m_sound = nullptr;
		}

	}

	void Sound::Play(bool loop)
	{
		if (m_channel != nullptr)
		{
			m_channel->isPlaying(&m_is_playing);
		}
		if (m_is_playing == false)
		{
			FMOD_RESULT ret = m_system->playSound(m_sound, nullptr, false, &m_channel);
			if (ret == FMOD_OK)
			{
				if (loop)
					m_channel->setMode(FMOD_LOOP_NORMAL);
				else
					m_channel->setMode(FMOD_LOOP_OFF);
			}
		}
	}

	void Sound::Stop()
	{
		if (m_channel != nullptr)
		{
			m_channel->stop();
		}
	}
	void Sound::Pause()
	{
		m_channel->isPlaying(&m_is_playing);
		if (m_is_playing)
		{
			bool is_paused;
			m_channel->getPaused(&is_paused);
			m_channel->setPaused(!is_paused);
		}
		
	}
	void Sound::VolumeDown(float value)
	{
		m_volume -= value;
		SetVolume(m_volume);
	}
	void Sound::VolumeUp(float value)
	{
		m_volume += value;
		SetVolume(m_volume);
	}
	void Sound::SetVolume(float value)
	{
		if (m_channel != nullptr)
		{
			m_volume = std::clamp(value, 0.f, 1.0f);
			m_channel->setVolume(m_volume);
		}
	}
}