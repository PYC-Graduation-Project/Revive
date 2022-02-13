#include "stdafx.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/mesh_octree.h"

namespace client_fw
{
	OctreeManager* OctreeManager::s_instance = nullptr;

	OctreeManager::OctreeManager()
	{
		s_instance = this;
	}

	void OctreeManager::RegisterMeshOctree(const SPtr<MeshOctree>& octree)
	{
		if (octree != nullptr)
		{
			if (m_mesh_octree != nullptr)
				UnregisterMeshOctree();
			m_mesh_octree = octree;
			m_mesh_octree->Initialize();
		}
	}

	void OctreeManager::UnregisterMeshOctree()
	{
		if (m_mesh_octree != nullptr)
		{
			m_mesh_octree->Shutdown();
			m_mesh_octree = nullptr;
		}
	}

	void OctreeManager::RegisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (m_mesh_octree != nullptr)
			m_mesh_octree->RegisterMeshComponent(mesh);
	}

	void OctreeManager::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (m_mesh_octree != nullptr)
			m_mesh_octree->UnregisterMeshComponent(mesh);
	}

	void OctreeManager::SetVisibilityFromCamera(const SPtr<CameraComponent>& camera)
	{
		if (m_mesh_octree != nullptr)
			m_mesh_octree->SetVisibilityFromCamera(camera);
	}
}