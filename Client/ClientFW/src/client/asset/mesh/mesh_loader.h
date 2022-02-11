#pragma once

namespace client_fw
{
	class Mesh;
	class StaticMesh;
	class Skeleton;
	class SkeletalMesh;

	class MeshLoader
	{
	public:
		MeshLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& path, const std::string& extension);

		virtual SPtr<SkeletalMesh> LoadRev(const std::string& path, const std::string& extension);
		
		//�ִϸ��̼ǰ� �������� ������ ����(rev����)�� ����
		void LoadFrameHierArchy(FILE* rev_file,SPtr<Skeleton>& skeleton,SPtr<SkeletalMesh>& mesh);
		
		//�޽ð� ������ �ִ°� ���ľ��ϴµ� �Ű������� Mesh�� �־ �����͸�������	
		//CopyMemory()�Ҷ� ����Ⱑ �Ͼ�� ���������� ���� �޽� �����͸� ����� ���̴�.
		//void LoadMeshFromFile(std::ifstream& rev_file,SPtr<Mesh>& mesh)�϶� ������ ����
		//SPtr<SkeletalMesh>���� SPtr<Mesh>&�� �ٲܼ� �����ϴ� &&�ΰ����̴ϱ�ǳ�
		void LoadMeshFromFile(FILE* rev_file, SPtr<Mesh>&& mesh);
		void LoadSkinDeformations(FILE* rev_file, SPtr<SkeletalMesh>& mesh);

		virtual SPtr<StaticMesh> LoadObj(const std::string& path, const std::string& extension);

	};
	//int ReadFromFile(FILE* file, std::stringstream* word);
	int ReadStringFromFile(FILE* file , std::string* word);

}


