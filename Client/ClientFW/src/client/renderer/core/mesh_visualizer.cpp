#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/util/core/camera_component.h"
#include "client/object/component/core/render_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	void MeshVisualizer::UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera,
		std::function<void(const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye)> function,
		const std::function<bool(const SPtr<RenderComponent>)>& trigger_function)
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
				function(bounding_frustum, type, root_node, eye);
			}
		}

		const auto& movable_render_comps = VisualOctreeManager::GetOctreeManager().GetMovableRenderComps();

		for (const auto& render_comp : movable_render_comps)
		{
			if (trigger_function(render_comp) &&
				bounding_frustum.Intersects(*render_comp->GetOrientedBox()))
			{
				render_comp->SetVisiblity(true);
				render_comp->UpdateLevelOfDetail(eye);
			}
		}
	}

	void MeshVisualizer::UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
		ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye,
		const std::function<bool(const SPtr<RenderComponent>)>& trigger_function)
	{
		if (node->child_nodes[0] == nullptr)
		{
			switch (type)
			{
			case DirectX::INTERSECTS:
				for (const auto& render_cmp : node->render_components)
				{
					if (trigger_function(render_cmp))
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
					if (trigger_function(render_cmp))
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
					UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i], eye, trigger_function);
			}
			break;
		}
		case DirectX::CONTAINS:
			for (UINT i = 0; i < 8; ++i)
				UpdateVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i], eye, trigger_function);
			break;
		default:
			break;
		}
	}

	void MeshVisualizer::UpdateVisibilityFromRenderCamera(const SPtr<CameraComponent>& camera)
	{
		auto trigger_function = 
			[](const SPtr<RenderComponent>& render_cmp)
		{
			return (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};

		UpdateVisibilityFromCamera(camera, [trigger_function](const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye)
			{
				UpdateVisibilityFromCamera(bounding_frustum, type, node, eye, trigger_function);
			}, trigger_function);
	}

	void MeshVisualizer::UpdateVisibilityFromShadowCamera(const SPtr<CameraComponent>& camera)
	{
		auto trigger_function =
			[](const SPtr<RenderComponent>& render_cmp)
		{
			//Mesh를 제외하면 그림자를 생성하지 않는다.
			return
				(render_cmp->GetRenderType() == eRenderType::kMesh)
				&& (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};

		UpdateVisibilityFromCamera(camera, [trigger_function](const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye)
			{
				UpdateVisibilityFromCamera(bounding_frustum, type, node, eye, trigger_function);
			}, trigger_function);
	}
}
