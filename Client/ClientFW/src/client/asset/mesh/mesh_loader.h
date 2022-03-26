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
		
	private:
		UINT m_mesh_count = 0;
		bool is_single_mesh = true;
	private:
		void SaveRevData(SPtr<SkeletalMesh>& s_mesh, const UINT& lod, std::vector<MeshData>& mesh_data) const;

		//�������� ������ ����(rev����)�� ����

		//true��ȯ�� �޽������� ���� �о��� Animation ������ ���� �غ� ��ٴ� ���� �ǹ���
		bool LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, std::vector<MeshData>& mesh_data, const std::string& path) const;

		void LoadMeshFromRevFile(FILE* rev_file, std::vector<MeshData>& mesh_data) const;
		void LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data) const;

		//���⼭�� �����
		void AddRevMaterial(std::map<std::string, SPtr<Material>>& materials, const std::string& mtl_name, const std::string& parent_path) const;

		void AddMesh() const { static_cast<UINT>(m_mesh_count)++; }
		std::vector<MeshData> CreateMeshData() const;
		int ReadStringFromFile(FILE* file, std::string* word) const;
		//������ Frame�̳� Frame == Bone ����Ѵ�
		//fbx������ frame != Bone
	};
	

}


