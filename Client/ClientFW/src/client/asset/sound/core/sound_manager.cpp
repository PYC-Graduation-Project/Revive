#include "stdafx.h"
#include "client/util/fmod/fmod.hpp"
#include "client/util/fmod/fmod_errors.h"
#pragma comment(lib, "../ClientFW/src/lib/x64/fmod_vc.lib")

#include "client/asset/sound/core/sound_manager.h"
#include "client/asset/sound/sound.h"


namespace client_fw
{
	SoundManager* SoundManager::s_instance = nullptr;

	SoundManager::SoundManager()
	{
		s_instance = this;
	}

	SoundManager::~SoundManager()
	{
	}

	bool SoundManager::Initialize()
	{
		FMOD_RESULT ret = FMOD::System_Create(&m_sound_system);

		if (ret != FMOD_OK)
		{
			LOG_ERROR("FMOD_SYSTEM Create Fail!");
			return false;
		}

		ret = m_sound_system->init(32, FMOD_INIT_NORMAL, 0);

		if (ret != FMOD_OK)
		{
			LOG_ERROR("FMOD_SYSTEM Initialize Fail!");
			return false;
		}

		return true;
	}

	void SoundManager::Update(float delta_time)
	{
		m_sound_system->update();
		for (auto data : m_sound_list)
			data.second->Update(delta_time);
	}

	void SoundManager::Shutdown()
	{
		for (auto data : m_sound_list)
		{
			data.second->Shutdown();
			//delete data.second;
		}
		m_sound_list.clear();

		m_sound_system->close();
		m_sound_system->release();
		s_instance = nullptr;
	}

	SPtr<Sound> SoundManager::LoadSound(const std::string& path, const std::string& name)
	{
		FMOD_RESULT ret;

		std::string stem = std::filesystem::path(path).stem().string();
		std::string extension = std::filesystem::path(path).extension().string();
		std::string parent_path = std::filesystem::path(path).parent_path().string();

		//for(auto data: m_sound_list)
		SPtr<Sound> sound = CreateSPtr<Sound>(m_sound_system, m_index, path);
		ret = m_sound_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &(sound->m_sound));
		
		if (ret != FMOD_OK)
		{
			LOG_WARN("Sound Load Fail {0}", path);
			return nullptr;
		}

		m_sound_list.insert({ name, sound });

		return sound;
	}
	SPtr<Sound> SoundManager::GetSound(const std::string& name)
	{
		auto iter = m_sound_list.find(name);
		if (iter != m_sound_list.end())
			return (*iter).second;

		return nullptr;
	}

	void SoundManager::Play( eSoundType sound_type, const std::string& name, bool loop)
	{
		const auto sound = GetSound(name);
		m_current_sound_name = name;
		if (sound_type == eSoundType::kBackGroundSound)
		{
			if (m_bgm_channel != nullptr)
				m_bgm_channel->isPlaying(&m_is_playing);
			if (m_is_playing)
				m_bgm_channel->stop();
			FMOD_RESULT ret = m_sound_system->playSound(sound->m_sound, nullptr, false, &m_bgm_channel);
			if (ret == FMOD_OK)
			{
				if (loop)
					m_bgm_channel->setMode(FMOD_LOOP_NORMAL);
				else
					m_bgm_channel->setMode(FMOD_LOOP_OFF);
			}
		}
		if (sound_type == eSoundType::kEffectSound)
		{
		
			FMOD_RESULT ret = m_sound_system->playSound(sound->m_sound, nullptr, false, &m_effect_channel);
			if (ret == FMOD_OK)
			{
				if (loop)
					m_effect_channel->setMode(FMOD_LOOP_NORMAL);
				else
					m_effect_channel->setMode(FMOD_LOOP_OFF);
			}
		}
		

		
		
	}
	void SoundManager::Stop()
	{
		if (m_bgm_channel != nullptr)
		{
			m_bgm_channel->stop();
		}
	}
	void SoundManager::Pause()
	{
		m_bgm_channel->isPlaying(&m_is_playing);
		if (m_is_playing)
		{
			bool is_paused;
			m_bgm_channel->getPaused(&is_paused);
			m_bgm_channel->setPaused(!is_paused);
		}

	}
	void SoundManager::VolumeDown(float value)
	{
		m_volume -= value;
		SetVolume(m_volume);
	}
	void SoundManager::VolumeUp(float value)
	{
		m_volume += value;
		SetVolume(m_volume);
	}
	void SoundManager::SetVolume(float value)
	{
		if (m_bgm_channel != nullptr)
		{
			m_volume = std::clamp(value, 0.f, 1.0f);
			m_bgm_channel->setVolume(m_volume);
		}
	}
}
