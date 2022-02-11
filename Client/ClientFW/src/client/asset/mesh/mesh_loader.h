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

	class RevLoader : public MeshLoader
	{
		RevLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& path, const std::string& extension) override;
		virtual SPtr<SkeletalMesh> LoadRev(const std::string& path, const std::string& extension);

		//애니메이션과 계층구조 형태의 파일(rev파일)을 읽음
		void LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh);

		//메시가 여러개 있는걸 합쳐야하는데 매개변수로 Mesh를 넣어서 데이터를넣으면	
		//CopyMemory()할때 덮어쓰기가 일어나서 마지막으로 읽은 메시 데이터만 저장될 것이다.
		//void LoadMeshFromFile(std::ifstream& rev_file,SPtr<Mesh>& mesh)일때 빨간줄 생김
		//SPtr<SkeletalMesh>에서 SPtr<Mesh>&로 바꿀수 없습니다 &&두개붙이니까되네
		//void LoadMeshFromFile(FILE* rev_file, SPtr<Mesh>&& mesh);
		void LoadMeshFromFile(FILE* rev_file, MeshData& mesh_data);
		void LoadSkinDeformations(FILE* rev_file, SPtr<SkeletalMesh>& mesh);

		int ReadStringFromFile(FILE* file, std::string* word);
	};
	//int ReadFromFile(FILE* file, std::stringstream* word);

}


