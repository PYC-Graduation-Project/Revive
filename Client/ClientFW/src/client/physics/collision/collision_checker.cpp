#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/collision/collision_checker.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/octree.h"
#include "client/physics/collision/collisioner/collisioner.h"

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
					const auto& comp = node->movable_scene_components[i];
					
					for (size_t j = i + 1; j < node->movable_scene_components.size(); ++j)
					{
						const auto& other = node->movable_scene_components[j];
						if (comp->GetCollisioner() != nullptr && other->GetCollisioner() != nullptr &&
							comp->GetOwner().lock() != other->GetOwner().lock())
						{
							comp->GetCollisioner()->CheckCollisionWithOtherComponent(other);
						}
					}

					for (const auto& static_comp : node->static_scene_components)
					{
						if (comp->GetCollisioner() != nullptr && static_comp->GetCollisioner() != nullptr &&
							comp->GetOwner().lock() != static_comp->GetOwner().lock())
						{
							comp->GetCollisioner()->CheckCollisionWithOtherComponent(static_comp);
						}
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
