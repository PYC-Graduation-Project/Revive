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

		//애니메이션과 계층구조 형태의 파일(rev파일)을 읽음

		//true반환시 메시정보를 전부 읽었고 Animation 정보를 읽을 준비가 됬다는 것을 의미함
		bool LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh, const std::string& path) const;

		//void LoadMeshFromFile(FILE* rev_file, SPtr<Mesh>&& mesh);
		void LoadMeshFromRevFile(FILE* rev_file, MeshData& mesh_data) const;
		void LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data) const;

		//메시가 기존에 있는거면 저장된거 찾아서 전달해야함
		SPtr<AnimationSequence> LoadAnimation(FILE* file,const SPtr<Skeleton>& skeleton) const;

		int ReadStringFromFile(FILE* file, std::string* word) const;

		//마테리얼을 로드하는게아닌 생성임, 따라서 Material_Loader에 정의하지 않음,또한 여기서만 사용함
		std::map<std::string, SPtr<Material>> CreateRevMaterial(const std::string& mtl_name, const std::string& parent_path) const;

	};
	

}


