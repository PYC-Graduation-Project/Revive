#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/mesh_octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	void MeshVisualizer::UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera)
	{
		const auto& mesh_octree = OctreeManager::GetOctreeManager().GetMeshOctree();
		if (mesh_octree != nullptr)
		{
			const auto& root_node = mesh_octree->GetRootNode();
			const auto& out_of_range_meshes = mesh_octree->GetOutOfRangeMeshes();
			const auto& movable_meshes = mesh_octree->GetMovableMeshes();


			const auto& bounding_frustum = camera->GetBoudingFrustum();

			ContainmentType type = bounding_frustum.Contains(root_node->bounding_box);
			if (type != ContainmentType::DISJOINT)
			{
				UpdateVisibilityFromCamera(bounding_frustum, type, root_node);
			}

			for (const auto& mesh : out_of_range_meshes)
			{
				if (bounding_frustum.Intersects(mesh->GetOrientdBox()))
					mesh->SetVisiblity(true);
			}

			for (const auto& mesh : movable_meshes)
			{
				if (bounding_frustum.Intersects(mesh->GetOrientdBox()))
					mesh->SetVisiblity(true);
			}
		}
	}

	void MeshVisualizer::UpdateVisibilityFromCamera(const BoundingFrustum& bounding_frustum, ContainmentType type, const SPtr<MeshTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			switch (type)
			{
			case DirectX::INTERSECTS:
				for (const auto& mesh : node->mesh_components)
				{
					if (bounding_frustum.Intersects(mesh->GetOrientdBox()))
						mesh->SetVisiblity(true);
				}
				break;
			case DirectX::CONTAINS:
				for (const auto& mesh : node->mesh_components)
				{
					mesh->SetVisiblity(true);
				}
				break;
			default:
				break;
			}
			return;
		}

		switch (type)
		{
		case DirectX::INTERSECTS:
		{
			ContainmentType type;
			for (UINT i = 0; i < 8; ++i)
			{
				type = bounding_frustum.Contains(node->child_nodes[i]->bounding_box);
				if (type != ContainmentType::DISJOINT)
					UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i]);
			}
			break;
		}
		case DirectX::CONTAINS:
			for (UINT i = 0; i < 8; ++i)
				UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i]);
			break;
		default:
			break;
		}

	}
}
