#include "stdafx.h"
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
		const auto& mesh_octree = OctreeManager::GetOctreeManager().GetMeshOctree();
		if (mesh_octree != nullptr)
		{
			const auto& root_node = mesh_octree->GetRootNode();
			const auto& out_of_range_meshes = mesh_octree->GetOutOfRangeMeshes();
			const auto& movable_meshes = mesh_octree->GetMovableMeshes();

			auto CheckStaticMesh([this, &root_node, &out_of_range_meshes]
			(const SPtr<MeshComponent>& mesh, const BOrientedBox& obox)
				{
					if (mesh->GetOrientdBox().Intersects(root_node->bounding_box))
						CheckCollisions(mesh, mesh->GetOrientdBox(), root_node);
					else
						CheckCollisions(mesh, mesh->GetOrientdBox(), out_of_range_meshes);
				});

			for (size_t i = 0; i < movable_meshes.size() - 1; ++i)
			{
				const auto& mesh = movable_meshes[i];
				CheckStaticMesh(mesh, mesh->GetOrientdBox());
				for (size_t j = i + 1; j < movable_meshes.size(); ++j)
				{
					const auto& movable_mesh = movable_meshes[j];
					if (mesh->GetOrientdBox().Intersects(movable_mesh->GetOrientdBox()))
					{
						//이벤트 실행
						LOG_INFO("{0} col {1}",
							mesh->GetOwner().lock()->GetName(),
							movable_mesh->GetOwner().lock()->GetName());
					}
				}
			}

			if (movable_meshes.empty() == false)
			{
				const auto& mesh = movable_meshes[movable_meshes.size() - 1];
				CheckStaticMesh(mesh, mesh->GetOrientdBox());
			}
		}
	}

	void CollisionChecker::CheckCollisions(const SPtr<MeshComponent>& mesh, 
		const BOrientedBox& obox, const SPtr<MeshTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			CheckCollisions(mesh, obox, node->mesh_components);
			return;
		}

		for (UINT i = 0; i < 8; ++i)
		{
			if (obox.Intersects(node->child_nodes[i]->bounding_box))
				CheckCollisions(mesh, obox, node->child_nodes[i]);
		}
	}

	void CollisionChecker::CheckCollisions(const SPtr<MeshComponent>& mesh, const BOrientedBox& obox, const std::vector<SPtr<MeshComponent>>& static_meshes)
	{
		for (const auto& static_mesh : static_meshes)
		{
			if (mesh->GetOrientdBox().Intersects(static_mesh->GetOrientdBox()))
			{
				//이벤트 실행
				LOG_INFO("{0} col {1}",
					mesh->GetOwner().lock()->GetName(),
					static_mesh->GetOwner().lock()->GetName());
			}
		}
	}
}
