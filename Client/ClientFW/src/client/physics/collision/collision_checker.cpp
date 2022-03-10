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
				for (const auto& [type, mov_comps] : node->movable_scene_components)
				{
					for (const auto& mov_comp : mov_comps)
					{
						const auto& col_types = mov_comp->GetCollisioner()->GetCollisionInfo().collisionable_types;

						auto iter = std::lower_bound(col_types.cbegin(), col_types.cend(), type);

						for (iter; iter != col_types.cend(); ++iter)
						{
							for (const auto& other_comp : node->movable_scene_components[*iter])
							{
								const auto& other_col_types = other_comp->GetCollisioner()->GetCollisionInfo().collisionable_types;
								if (other_col_types.find(type)!= other_col_types.cend() && 
									mov_comp->GetOwner().lock() != other_comp->GetOwner().lock() &&
									mov_comp->GetCollisioner() != nullptr && other_comp->GetCollisioner() != nullptr)
								{
									mov_comp->GetCollisioner()->CheckCollisionWithOtherComponent(other_comp);
								}
							}
						}

						for (const auto& other_type : col_types)
						{
							for (const auto& other_comp : node->static_scene_components[other_type])
							{
								const auto& other_col_types = other_comp->GetCollisioner()->GetCollisionInfo().collisionable_types;
								if (other_col_types.find(type) != other_col_types.cend() &&
									mov_comp->GetOwner().lock() != other_comp->GetOwner().lock() &&
									mov_comp->GetCollisioner() != nullptr && other_comp->GetCollisioner() != nullptr)
								{
									mov_comp->GetCollisioner()->CheckCollisionWithOtherComponent(other_comp);
								}
							}
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
