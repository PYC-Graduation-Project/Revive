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
		virtual SPtr<StaticMesh> LoadObj(const std::string& path, const std::string& extension);

	};

	struct MeshData;
	struct BoneData; //�����͸� ��� ������,�ٱ����� �ѹ��� Set�ϱ����� �߰�

	class RevLoader : public MeshLoader
	{
	public:

		RevLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& path, const std::string& extension) override;
		virtual SPtr<SkeletalMesh> LoadRev(const std::string& path, const std::string& extension);

		//�ִϸ��̼ǰ� �������� ������ ����(rev����)�� ����

		//true��ȯ�� �޽������� ���� �о��� Animation ������ ���� �غ� ��ٴ� ���� �ǹ���
		bool LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh);

		//void LoadMeshFromFile(FILE* rev_file, SPtr<Mesh>&& mesh);
		void LoadMeshFromRevFile(FILE* rev_file, MeshData& mesh_data);
		void LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data);

		int ReadStringFromFile(FILE* file, std::string* word);
	};
	//int ReadFromFile(FILE* file, std::stringstream* word);

}


