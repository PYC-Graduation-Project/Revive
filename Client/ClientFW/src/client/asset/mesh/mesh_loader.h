#pragma once

namespace client_fw
{
	class Mesh;
	class StaticMesh;

	class MeshLoader
	{
	public:
		MeshLoader() = default;

		virtual SPtr<Mesh> LoadMesh(const std::string& name, const std::string& path, const std::string& extension);

		virtual SPtr<StaticMesh> LoadObj(const std::string& name, const std::string& path, const std::string& extension);
		virtual SPtr<StaticMesh> LoadRev(const std::string& name, const std::string& path, const std::string& extension);
		
		void LoadFrameHierArchy(std::ifstream* rev_file);
		int ReadFromFile(std::ifstream* file, std::stringstream* word);
	};
}


