#pragma once

namespace FMOD
{
	class System;
}

namespace revive
{
	using namespace client_fw;

	//���� ��ü�� �������ϰ� 1��1 ����, SoundManager �̱��� ����(?), Sound Ŭ���� ����
	class Sound;

	class SoundManager { //IBaseManager ���?
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