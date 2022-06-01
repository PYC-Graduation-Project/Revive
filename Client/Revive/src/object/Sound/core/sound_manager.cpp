#include <include/client_core.h>
#include "include/fmod.hpp"
#include "include/fmod_errors.h"
#pragma comment(lib, "src/lib/x64/fmod_vc.lib")

#include "object/sound/core/sound_manager.h"
#include "object/sound/sound.h"


namespace revive
{
	FMOD::System* SoundManager::m_system;

	SoundManager::SoundManager()
	{
	}

	bool SoundManager::Initialize()
	{
		FMOD_RESULT ret = FMOD::System_Create(&m_system);

		if (ret != FMOD_OK)
		{
			LOG_ERROR("FMOD_SYSTEM Create Fail!");
			return false;
		}

		ret = m_system->init(32, FMOD_INIT_NORMAL, 0);

		if (ret != FMOD_OK)
		{
			LOG_ERROR("FMOD_SYSTEM Initialize Fail!");
			return false;
		}

		return true;
	}

	void SoundManager::Update()
	{
		m_system->update();
	}

	SPtr<Sound> SoundManager::LoadSound(const std::string& path)
	{
		FMOD_RESULT ret;

		std::string stem = std::filesystem::path(path).stem().string();
		std::string extension = std::filesystem::path(path).extension().string();
		std::string parent_path = std::filesystem::path(path).parent_path().string();

		//for(auto data: m_sound_list)
		SPtr<Sound> sound = CreateSPtr<Sound>(m_system, m_index, stem);
		ret = m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &(sound->m_sound));
		
		if (ret != FMOD_OK)
		{
			LOG_WARN("Sound Load Fail {0}", path);
			return nullptr;
		}

		m_sound_list.insert({ m_index++, sound });

		return sound;
	}
	SPtr<Sound> SoundManager::GetSound(UINT index)
	{
		auto iter = m_sound_list.find(index);
		if (iter != m_sound_list.end())
			return (*iter).second;

		return nullptr;
	}
}
