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

	
}