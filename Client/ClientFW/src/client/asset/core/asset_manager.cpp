#include "stdafx.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/core/asset.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/material/material.h"
#include "client/asset/material/material_loader.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/animation/animation_sequence.h"

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

	void AssetManager::Initialize(UPtr<MeshLoader>&& mesh_loader, UPtr<MaterialLoader>&& material_loader,
		UPtr<TextureLoader>&& texture_loader, bool level_cache)
	{
		m_mesh_loader = std::move(mesh_loader);
		m_material_loader = std::move(material_loader);
		m_texture_loader = std::move(texture_loader);
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
			asset = m_mesh_loader->LoadMesh(path, extension);
			if (asset != nullptr)
			{
				SaveAsset(eAssetType::kMesh, stem, path, extension, asset);
				RenderResourceManager::GetRenderResourceManager().RegisterMesh(std::static_pointer_cast<Mesh>(asset));
			}
		}

		return (asset == nullptr) ? nullptr : std::static_pointer_cast<Mesh>(asset);
	}

	SPtr<Material> AssetManager::LoadMaterial(const std::string& mtl_path)
	{
		auto asset = LoadAsset(eAssetType::kMaterial, mtl_path);
		return (asset == nullptr) ? nullptr : std::static_pointer_cast<Material>(asset);
	}

	std::map<std::string, SPtr<Material>> AssetManager::LoadMaterials(const std::string& path)
	{
		std::string extension = file_help::GetExtentionFromPath(path);
		std::map<std::string, SPtr<Material>> materials = m_material_loader->LoadMaterials(path, extension);

		for (const auto& [name, material] : materials)
		{
			if (m_asset_caches[eAssetType::kMaterial].find(material->GetPath()) ==
				m_asset_caches[eAssetType::kMaterial].cend())
			{
				m_asset_caches[eAssetType::kMaterial].insert({ material->GetPath(), material });
				RenderResourceManager::GetRenderResourceManager().RegisterMaterial(material);
			}
		}
			
		return materials;
	}

	SPtr<ExternalTexture> AssetManager::LoadTexture(const std::string& path)
	{
		auto asset = LoadAsset(eAssetType::kTexture, path);
		if (asset == nullptr)
		{
			std::string stem = file_help::GetStemFromPath(path);
			std::string extension = file_help::GetExtentionFromPath(path);
			asset = m_texture_loader->LoadTexture(path, extension);

			if (asset != nullptr)
			{
				LOG_INFO("Stem : {0}, Path : {1}, extension : {2}", stem, path, extension);
				SaveAsset(eAssetType::kTexture, stem, path, extension, asset);
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(std::static_pointer_cast<ExternalTexture>(asset));
			}
		}

		return (asset == nullptr) ? nullptr : std::static_pointer_cast<ExternalTexture>(asset);
	}

	SPtr<AnimationSequence> AssetManager::LoadAnimation(FILE* file, const SPtr<Skeleton>& skeleton, const std::string& path)
	{
		auto asset = LoadAsset(eAssetType::kAnimation, path);
		if (asset == nullptr)
		{
			std::string stem = file_help::GetStemFromPath(path);
			std::string extension = file_help::GetExtentionFromPath(path);
			asset = m_rev_loader->LoadAnimation(file,skeleton);
			if (asset != nullptr)
			{
				LOG_INFO("Stem : {0}, Path : {1}, extension : {2}", stem, path, extension);
				SaveAsset(eAssetType::kAnimation, stem, path, extension, asset);
			}

		}
		return (asset == nullptr) ? nullptr : std::static_pointer_cast<AnimationSequence>(asset);
	}

	SPtr<AnimationSequence> AssetManager::LoadAnimation(const std::string& path, const SPtr<Skeleton>& skeleton)
	{
		auto asset = LoadAsset(eAssetType::kAnimation, path);
		if (asset == nullptr)
		{
			std::string stem = file_help::GetStemFromPath(path);
			std::string extension = file_help::GetExtentionFromPath(path);
			
			FILE* animation_file = m_rev_loader->LoadRevForAnimation(path, extension);
			asset = m_rev_loader->LoadAnimation(animation_file,skeleton);
			if (asset != nullptr)
			{
				LOG_INFO("Stem : {0}, Path : {1}, extension : {2}", stem, path, extension);
				SaveAsset(eAssetType::kAnimation, stem, path, extension, asset);
			}

		}
		return (asset == nullptr) ? nullptr : std::static_pointer_cast<AnimationSequence>(asset);
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

		std::string GetParentPathFromPath(const std::string& path)
		{
			return std::filesystem::path(path).parent_path().string();
		}
	}
}
