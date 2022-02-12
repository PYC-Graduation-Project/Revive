#include "stdafx.h"
#include "client/util/octree/mesh_octree.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	VisualOctree::VisualOctree(float width, Vec3 pos, UINT depth)
		: MeshOctree(width, pos, depth)
	{
	}

	void VisualOctree::RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<VisualTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			node->mesh_components.push_back(mesh);
			mesh->AddVisualTreeNode(node);
			return;
		}

		for (UINT i = 0; i < 8; ++i)
		{
			if (mesh->GetOrientedBox()->Intersects(node->child_nodes[i]->bounding_box))
				RegisterMeshComponent(mesh, node->child_nodes[i]);
		}
	}

	CollisionOctree::CollisionOctree(float width, Vec3 pos, UINT depth)
		: MeshOctree(width, pos, depth)
	{
	}

	void CollisionOctree::RegisterSceneComponent(const SPtr<SceneComponent>& scene_comp, const SPtr<CollisionTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			if (scene_comp->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
				node->movable_scene_components.push_back(scene_comp);
			else
				node->static_scene_components.push_back(scene_comp);
			scene_comp->AddCollisionTreeNode(node);
			return;
		}

		for (UINT i = 0; i < 8; ++i)
		{
			if (scene_comp->GetOrientedBox()->Intersects(node->child_nodes[i]->bounding_box))
				RegisterSceneComponent(scene_comp, node->child_nodes[i]);
		}
	}
}
