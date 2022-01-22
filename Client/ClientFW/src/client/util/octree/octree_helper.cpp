#include "stdafx.h"
#include "client/util/octree/octree_helper.h"
#include "client/util/octree/octree_manager.h"

namespace client_fw
{
	void OctreeHelper::RegisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		OctreeManager::GetOctreeManager().RegisterMeshComponent(mesh);
	}

	void OctreeHelper::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		OctreeManager::GetOctreeManager().UnregisterMeshComponent(mesh);
	}
}