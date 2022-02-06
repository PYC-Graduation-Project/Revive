#include "stdafx.h"
#include "client/asset/mesh/vertex.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/core/collision_checker.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/mesh_octree.h"

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
					{
						const auto& movable_mesh = node->movable_mesh_components[j];

						if (mesh->GetOrientedBox().Intersects(movable_mesh->GetOrientedBox()))
							CheckCollisionComplexity(mesh, movable_mesh);
					}

					for (const auto& static_mesh : node->static_mesh_components)
					{
						if (mesh->GetOrientedBox().Intersects(static_mesh->GetOrientedBox()))
							CheckCollisionComplexity(mesh, static_mesh);
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

	void CollisionChecker::CheckCollisionComplexity(const SPtr<MeshComponent>& mesh1, const SPtr<MeshComponent>& mesh2)
	{
		LOG_INFO("{0} col {1}",
			mesh1->GetOwner().lock()->GetName(),
			mesh2->GetOwner().lock()->GetName());
	}
}
