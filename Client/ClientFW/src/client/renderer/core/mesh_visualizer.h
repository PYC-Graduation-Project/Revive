#pragma once

namespace DirectX
{
	enum ContainmentType;
}

namespace client_fw
{
	template <class Type>
	class Bounding;
	class BFrustum;

	class CameraComponent;
	class RenderComponent;
	struct VisualTreeNode;

	class MeshVisualizer final
	{
	private:
		template <class Type>
		static void UpdateVisibilityFromVisualTreeNode(const Bounding<Type>& bounding,
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
							if (bounding.Intersects(*render_cmp->GetOrientedBox()))
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
					type = bounding.Contains(node->child_nodes[i]->bounding_box);
					if (type != ContainmentType::DISJOINT)
						UpdateVisibilityFromVisualTreeNode(bounding, type, node->child_nodes[i], eye, trigger_function);
				}
				break;
			}
			case DirectX::CONTAINS:
				for (UINT i = 0; i < 8; ++i)
					UpdateVisibilityFromVisualTreeNode(bounding, type, node->child_nodes[i], eye, trigger_function);
				break;
			default:
				break;
			}
		}

		template <class Type>
		static void UpdateVisibility(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>)>& trigger_function)
		{
			const auto& visual_octrees = VisualOctreeManager::GetOctreeManager().GetVisualOctrees();
			for (const auto& octree : visual_octrees)
			{
				const auto& root_node = octree->GetRootNode();

				ContainmentType type = bounding.Contains(root_node->bounding_box);
				if (type != ContainmentType::DISJOINT)
				{
					UpdateVisibilityFromVisualTreeNode(bounding, type, root_node, eye, trigger_function);
				}
			}

			const auto& movable_render_comps = VisualOctreeManager::GetOctreeManager().GetMovableRenderComps();

			for (const auto& render_comp : movable_render_comps)
			{
				if (trigger_function(render_comp) &&
					bounding.Intersects(*render_comp->GetOrientedBox()))
				{
					render_comp->SetVisiblity(true);
					render_comp->UpdateLevelOfDetail(eye);
				}
			}
		}

	public:
		static void UpdateVisibilityFromRenderCamera(const SPtr<CameraComponent>& camera);
		static void UpdateVisibilityFromShadowCamera(const SPtr<CameraComponent>& camera);
		static void UpdateVisibliityFromShadowSphere(const Vec3& eye, float radius);
	};
}



