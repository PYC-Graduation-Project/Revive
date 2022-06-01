#pragma once

namespace FMOD
{
	class System;
}

namespace revive
{
	using namespace client_fw;

	//사운드 객체는 사운드파일과 1대1 대응, SoundManager 싱글톤 구현(?), Sound 클래스 구현
	class Sound;

	class SoundManager { //IBaseManager 상속?
	public:
		SoundManager();
		virtual ~SoundManager() = default;

		virtual bool Initialize();
		virtual void Update();

	private:

		UINT m_index = 0;

		static FMOD::System* m_system; //UPtr?


		std::map <UINT, SPtr<Sound>> m_sound_list;
	public:

		SPtr<Sound> LoadSound(const std::string& path);

		SPtr<Sound> GetSound(UINT index);
	};

}