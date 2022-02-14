#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/collision/collision_checker.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/octree.h"
#include "client/physics/collision/collisioner.h"

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
			if (node->movable_scene_components.empty() == false)
			{
				for (size_t i = 0; i < node->movable_scene_components.size(); ++i)
				{
					const auto& mesh = node->movable_scene_components[i];
					
					for (size_t j = i + 1; j < node->movable_scene_components.size(); ++j)
					{
						if (mesh->GetCollisioner() != nullptr && node->movable_scene_components[j]->GetCollisioner() != nullptr)
							mesh->GetCollisioner()->CheckCollisionWithOtherComponent(node->movable_scene_components[j]);
					}

					for (const auto& static_mesh : node->static_scene_components)
					{
						if (mesh->GetCollisioner() != nullptr && static_mesh->GetCollisioner() != nullptr)
							mesh->GetCollisioner()->CheckCollisionWithOtherComponent(static_mesh);
					}
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
