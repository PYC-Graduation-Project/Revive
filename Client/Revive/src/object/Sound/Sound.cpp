#include <include/client_core.h>
#include "include/fmod.hpp"
#include "include/fmod_errors.h"

#include "object/sound/sound.h"

namespace revive
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

	void Sound::Update()
	{
		if (m_sound != nullptr || m_channel != nullptr)
			return;
		m_sound->getLength(&m_size, FMOD_TIMEUNIT_MS);
		m_channel->getPosition(&m_pos, FMOD_TIMEUNIT_MS);
	}

	void Sound::SoundPlay(bool loop)
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
}