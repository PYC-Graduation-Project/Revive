#include "stdafx.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/core/asset.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/mesh_loader.h"

namespace client_fw
{
	AssetManager* AssetStore::s_asset_manager = nullptr;

	AssetManager::AssetManager()
	{
		AssetStore::s_asset_manager = this;
	}

	AssetManager::~AssetManager()
	{
	}

	void AssetManager::Initialize(UPtr<MeshLoader>&& mesh_loader, bool level_cache)
	{
		m_mesh_loader = std::move(mesh_loader);
		m_is_level_cache = level_cache;
	}

	void AssetManager::Shutdown()
	{
		for (const auto& [type, asset_cache] : m_asset_caches)
		{
			for (const auto& [path, asset] : asset_cache)
				asset->Shutdown();
		}
	}

	UINT AssetManager::GetNumOfAsset(eAssetType type) const
	{
		return static_cast<UINT>(m_asset_caches.at(type).size());
	}

	SPtr<Asset> AssetManager::LoadAsset(eAssetType type, const std::string& path) const
	{
		if (m_asset_caches.find(type) != m_asset_caches.cend())
		{
			auto iter = m_asset_caches.at(type).find(path);
			if (iter != m_asset_caches.at(type).cend())
				return iter->second;
		}
		return nullptr;
	}

	void AssetManager::SaveAsset(eAssetType type, const std::string& name, const std::string& path,
		const std::string& extension, const SPtr<Asset>& asset)
	{
		asset->SetAssetInfo({ name, path, extension });
		m_asset_caches[type].insert({ path, asset });
	}

	UINT AssetManager::GetNumOfMesh() const
	{
		return GetNumOfAsset(eAssetType::kMesh);
	}

	SPtr<Mesh> AssetManager::LoadMesh(const std::string& path)
	{
		auto asset = LoadAsset(eAssetType::kMesh, path);
		if (asset == nullptr)
		{
			std::string stem = file_help::GetStemFromPath(path);
			std::string extension = file_help::GetExtentionFromPath(path);
			asset = m_mesh_loader->LoadMesh(stem, path, extension);
			if (asset != nullptr)
				SaveAsset(eAssetType::kMesh, stem, path, extension, asset);
		}
		LOG_INFO("Load Mesh : {0}", asset->GetName());

		return (asset == nullptr) ? nullptr : std::reinterpret_pointer_cast<Mesh>(asset);
	}

	namespace file_help
	{
		std::string GetStemFromPath(const std::string& path)
		{
			return std::filesystem::path(path).stem().string();
		}

		std::string GetExtentionFromPath(const std::string& path)
		{
			return std::filesystem::path(path).extension().string();
		}
	}
}
