#pragma once

namespace FMOD
{
	class System;
}

namespace client_fw
{
	//���� ��ü�� �������ϰ� 1��1 ����, SoundManager �̱��� ����, Sound Ŭ���� ����
	class Sound;

	class SoundManager final
	{ 
	
	public:
		SoundManager();
		virtual ~SoundManager();

		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;

		bool Initialize();
		virtual void Update(float delta_time);
		virtual void Shutdown();

	private:
		static SoundManager* s_instance;

		UINT m_index = 0;

		FMOD::System* m_sound_system;

		std::map <UINT, SPtr<Sound>> m_sound_list;
	public:

		inline static SoundManager& GetSoundManager() { return *s_instance; }

		SPtr<Sound> LoadSound(const std::string& path);
		SPtr<Sound> GetSound(UINT index);
	};

}