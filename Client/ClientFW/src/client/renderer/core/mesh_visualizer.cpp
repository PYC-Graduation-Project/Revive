#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/component/core/render_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	void MeshVisualizer::UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera)
	{
		const auto& bounding_frustum = camera->GetBoudingFrustum();
		const auto& eye = camera->GetWorldPosition();

		const auto& visual_octrees = VisualOctreeManager::GetOctreeManager().GetVisualOctrees();
		for (const auto& octree : visual_octrees)
		{
			const auto& root_node = octree->GetRootNode();

			ContainmentType type = bounding_frustum.Contains(root_node->bounding_box);
			if (type != ContainmentType::DISJOINT)
			{
				UpdateVisibilityFromCamera(bounding_frustum, type, root_node, eye);
			}
		}

		const auto& movable_render_comps = VisualOctreeManager::GetOctreeManager().GetMovableRenderComps();

		for (const auto& render_comp : movable_render_comps)
		{
			if (bounding_frustum.Intersects(*render_comp->GetOrientedBox()))
			{
				render_comp->SetVisiblity(true);
				render_comp->UpdateLevelOfDetail(eye);
			}
		}
	}

	void MeshVisualizer::UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
		ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye)
	{
		if (node->child_nodes[0] == nullptr)
		{
			switch (type)
			{
			case DirectX::INTERSECTS:
				for (const auto& render_cmp : node->render_components)
				{
					if (render_cmp->IsVisible() == false)
					{
						if (bounding_frustum.Intersects(*render_cmp->GetOrientedBox()))
						{
							render_cmp->SetVisiblity(true);
							render_cmp->UpdateLevelOfDetail(eye);
						}
					}
				}
				break;
			case DirectX::CONTAINS:
				for (const auto& render_cmp : node->render_components)
				{
					if (render_cmp->IsVisible() == false)
					{
						render_cmp->SetVisiblity(true);
						render_cmp->UpdateLevelOfDetail(eye);
					}
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
					UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i], eye);
			}
			break;
		}
		case DirectX::CONTAINS:
			for (UINT i = 0; i < 8; ++i)
				UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i], eye);
			break;
		default:
			break;
		}

	}
}
