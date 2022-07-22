#pragma once

namespace FMOD
{
	class System;
	class Channel;
}

namespace client_fw
{
	enum class eSoundType
	{
		kBackGroundSound,
		kEffectSound
	};
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
		FMOD::Channel* m_bgm_channel;
		FMOD::Channel* m_effect_channel;

		std::unordered_map <std::string, SPtr<Sound>> m_sound_list;
		std::string m_current_sound_name;

		bool m_is_playing = false;
		float m_bgm_volume = 0.5f;
		float m_effect_volume = 0.5f;

	public:

		inline static SoundManager& GetSoundManager() { return *s_instance; }

		SPtr<Sound> LoadSound(const std::string& path, const std::string& name);
		SPtr<Sound> GetSound(const std::string& name);

		void Play(eSoundType sound_type, const std::string& name, bool loop = true);
		void Stop(); //bgm��
		void Pause(); //bgm��
		void VolumeDown(float value, eSoundType sound_type = eSoundType::kBackGroundSound);
		void VolumeUp(float value, eSoundType sound_type = eSoundType::kBackGroundSound);
		const float GetVolume(eSoundType sound_type = eSoundType::kBackGroundSound);
		void SetVolume(float value, eSoundType sound_type = eSoundType::kBackGroundSound);
	};

}