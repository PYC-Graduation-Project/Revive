#pragma once

namespace client_fw
{
	class Skeleton;
	class AnimationSequence;

	class AnimationLoader
	{
	public:
		AnimationLoader() = default;

		virtual SPtr<AnimationSequence> LoadAnimation(FILE* file, const SPtr<Skeleton>& skeleton) const;
		FILE* GetFilePointerForAnimation(const std::string& path, const std::string& extension) const;
	
	private:
		//사용할 함수들을 전부 static으로 만든다 static은 되도록 안쓰는게 좋을 것 같은데
		//코드 중복이 있겠지만 AnimationLoader에서 함수를 다시 만들어서 쓴다.

		int ReadStringFromFile(FILE* file, std::string* word) const;
		
		void ReadFrameHierArchy(FILE* file) const; //LoadFrameHierArchy()함수와 유사하나 파일을 저장하는부분은 없앴다.
		void ReadMesh(FILE* file) const;
		void ReadSkinDeformations(FILE* file) const;
	};
}
