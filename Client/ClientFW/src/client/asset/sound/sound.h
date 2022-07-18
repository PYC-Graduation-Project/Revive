#pragma once

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace client_fw
{
	class Sound
	{
	public:
		//Sound() = default;
		Sound(FMOD::System* system, UINT index, const std::string& name);
		virtual ~Sound() = default;

		virtual bool Initialize();
		virtual void Update(float delta_time);
		virtual void Shutdown();

	private:

		bool m_is_playing = false;

		float m_volume = 1.f;

		UINT m_size, m_pos, m_id;
		std::string m_name;

	public:

		FMOD::System* m_system = nullptr; 
		FMOD::Channel* m_channel = nullptr; 
		FMOD::Sound* m_sound = nullptr;
	
	public:

	};
}