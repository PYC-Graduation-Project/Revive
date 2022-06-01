#pragma once

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace revive
{
	class Sound
	{
	public:
		//Sound() = default;
		Sound(FMOD::System* system, UINT index, const std::string& name);
		virtual ~Sound() = default;

		virtual bool Initialize();
		virtual void Update();

	private:

		bool m_is_playing = false;

		UINT m_size, m_pos, m_id;
		std::string m_name;

	public:

		FMOD::System* m_system = nullptr; //WPTR
		FMOD::Channel* m_channel = nullptr; //SPTR?
		FMOD::Sound* m_sound = nullptr;//UPTR?
	
	public:

		void SoundPlay(bool loop);

	};
}