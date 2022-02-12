#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/collision/collision_checker.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/mesh_octree.h"
#include "client/physics/collision/mesh_collision_manager.h"

namespace client_fw
{
	CollisionChecker::CollisionChecker()
	{
	}

	void CollisionChecker::CheckCollisions()
	{
		const auto& collision_octrees = CollisionOctreeManager::GetOctreeManager().GetCollisionOctrees();
		for (const auto& octree : collision_octrees)
		{
			CheckCollisionInLeafNode(octree->GetRootNode());
		}
	}

	void CollisionChecker::CheckCollisionInLeafNode(const SPtr<CollisionTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			if (node->movable_mesh_components.empty() == false)
			{
				for (size_t i = 0; i < node->movable_mesh_components.size(); ++i)
				{
					const auto& mesh = node->movable_mesh_components[i];
					
					for (size_t j = i + 1; j < node->movable_mesh_components.size(); ++j)
						mesh->GetCollisionManager()->CheckCollisionWithOtherMesh(node->movable_mesh_components[j]->GetCollisionManager());

					for (const auto& static_mesh : node->static_mesh_components)
						mesh->GetCollisionManager()->CheckCollisionWithOtherMesh(static_mesh->GetCollisionManager());
				}
			}
		}
		else
		{
			for (const auto& child_node : node->child_nodes)
				CheckCollisionInLeafNode(child_node);
		}
	}
}
