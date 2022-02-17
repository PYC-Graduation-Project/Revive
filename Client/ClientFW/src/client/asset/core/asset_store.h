#pragma once

namespace client_fw
{
	class AssetManager;
	class Mesh;
	class Material;
	class Texture;

	class AssetStore final
	{
	public:
		static SPtr<Mesh> LoadMesh(const std::string& path);
		static SPtr<Material> LoadMaterial(const std::string& mtl_path);
		static std::map<std::string, SPtr<Material>> LoadMaterials(const std::string& path);
		static SPtr<Texture> LoadTexture(const std::string& path);

	private:
		friend AssetManager;
		static AssetManager* s_asset_manager;
	};
}


