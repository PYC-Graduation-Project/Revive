#pragma once

namespace client_fw
{
	class AssetManager;
	class Mesh;

	class AssetStore final
	{
	public:
		static SPtr<Mesh> LoadMesh(const std::string& path);


	private:
		friend AssetManager;
		static AssetManager* s_asset_manager;
	};
}


