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
		//����� �Լ����� ���� static���� ����� static�� �ǵ��� �Ⱦ��°� ���� �� ������
		//�ڵ� �ߺ��� �ְ����� AnimationLoader���� �Լ��� �ٽ� ���� ����.

		int ReadStringFromFile(FILE* file, std::string* word) const;
		
		void ReadFrameHierArchy(FILE* file) const; //LoadFrameHierArchy()�Լ��� �����ϳ� ������ �����ϴºκ��� ���ݴ�.
		void ReadMesh(FILE* file) const;
		void ReadSkinDeformations(FILE* file) const;
	};
}
