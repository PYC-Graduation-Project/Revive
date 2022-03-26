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
	struct BoneData; //데이터를 모두 모은뒤,바깥에서 한번에 Set하기위해 추가

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

		//계층구조 형태의 파일(rev파일)을 읽음

		//true반환시 메시정보를 전부 읽었고 Animation 정보를 읽을 준비가 됬다는 것을 의미함
		bool LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, std::vector<MeshData>& mesh_data, const std::string& path) const;

		void LoadMeshFromRevFile(FILE* rev_file, std::vector<MeshData>& mesh_data) const;
		void LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data) const;

		//여기서만 사용함
		void AddRevMaterial(std::map<std::string, SPtr<Material>>& materials, const std::string& mtl_name, const std::string& parent_path) const;

		void AddMesh() const { static_cast<UINT>(m_mesh_count)++; }
		std::vector<MeshData> CreateMeshData() const;
		int ReadStringFromFile(FILE* file, std::string* word) const;
		//원래는 Frame이나 Frame == Bone 취급한다
		//fbx에서는 frame != Bone
	};
	

}


