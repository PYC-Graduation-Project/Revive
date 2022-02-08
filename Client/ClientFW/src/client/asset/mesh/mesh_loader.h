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

		virtual SPtr<Mesh> LoadMesh(const std::string& name, const std::string& path, const std::string& extension);

		virtual SPtr<StaticMesh> LoadObj(const std::string& name, const std::string& path, const std::string& extension);
		virtual SPtr<SkeletalMesh> LoadRev(const std::string& name, const std::string& path, const std::string& extension);
		
		//�ִϸ��̼ǰ� �������� ������ ����(rev����)�� ����
		void LoadFrameHierArchy(std::ifstream& rev_file,SPtr<Skeleton>& skeleton,SPtr<SkeletalMesh>& mesh);
		void LoadMeshFromFIle(std::ifstream& rev_file); 
		//void LoadSkinDeformations

	};
	int ReadFromFile(std::ifstream& file, std::stringstream* word);
	int ReadStringFromFile(std::ifstream& file, std::string* word);

}


