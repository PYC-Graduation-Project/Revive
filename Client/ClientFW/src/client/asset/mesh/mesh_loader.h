#pragma once

namespace client_fw
{
	class Mesh;
	class StaticMesh;
	class Skeleton;
	class SkeletalMesh;
	class Material;
	class AnimationSequence;

	class MeshLoader
	{
	public:
		MeshLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& path, const std::string& extension) const;

		virtual SPtr<StaticMesh> LoadObj(const std::string& path, const std::string& extension) const;
	};

	struct MeshData;
	struct BoneData; //�����͸� ��� ������,�ٱ����� �ѹ��� Set�ϱ����� �߰�

	class RevLoader : public MeshLoader
	{
	public:

		RevLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& path, const std::string& extension)  const override;
		virtual SPtr<SkeletalMesh> LoadRev(const std::string& path, const std::string& extension) const;

		//�ִϸ��̼ǰ� �������� ������ ����(rev����)�� ����

		//true��ȯ�� �޽������� ���� �о��� Animation ������ ���� �غ� ��ٴ� ���� �ǹ���
		bool LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh, const std::string& path) const;

		//void LoadMeshFromFile(FILE* rev_file, SPtr<Mesh>&& mesh);
		void LoadMeshFromRevFile(FILE* rev_file, MeshData& mesh_data) const;
		void LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data) const;

		//�޽ð� ������ �ִ°Ÿ� ����Ȱ� ã�Ƽ� �����ؾ���
		SPtr<AnimationSequence> LoadAnimation(FILE* file,const SPtr<Skeleton>& skeleton) const;

		int ReadStringFromFile(FILE* file, std::string* word) const;

		//���׸����� �ε��ϴ°Ծƴ� ������, ���� Material_Loader�� �������� ����,���� ���⼭�� �����
		std::map<std::string, SPtr<Material>> CreateRevMaterial(const std::string& mtl_name, const std::string& parent_path) const;

	};
	

}


