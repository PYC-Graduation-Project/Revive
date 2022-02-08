#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/mesh_octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/core/bounding_mesh.h"

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
			const auto& eye = camera->GetOwner().lock()->GetPosition();

			ContainmentType type = bounding_frustum.Contains(root_node->bounding_box);
			if (type != ContainmentType::DISJOINT)
			{
				UpdateVisibilityFromCamera(bounding_frustum, type, root_node, eye);
			}

			for (const auto& mesh : out_of_range_meshes)
			{
				if (bounding_frustum.Intersects(mesh->GetOrientedBox()))
				{
					mesh->SetVisiblity(true);
					UpdateLevelOfDetail(mesh, eye);
				}
			}

			for (const auto& mesh : movable_meshes)
			{
				if (bounding_frustum.Intersects(mesh->GetOrientedBox()))
				{
					mesh->SetVisiblity(true);
					UpdateLevelOfDetail(mesh, eye);
				}
			}
		}
	}

	void MeshVisualizer::UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
		ContainmentType type, const SPtr<MeshTreeNode>& node, const Vec3& eye)
	{
		if (node->child_nodes[0] == nullptr)
		{
			switch (type)
			{
			case DirectX::INTERSECTS:
				for (const auto& mesh : node->mesh_components)
				{
					if (mesh->IsVisible() == false)
					{
						if (bounding_frustum.Intersects(mesh->GetOrientedBox()))
						{
							mesh->SetVisiblity(true);
							UpdateLevelOfDetail(mesh, eye);
						}
					}
				}
				break;
			case DirectX::CONTAINS:
				for (const auto& mesh : node->mesh_components)
				{
					if (mesh->IsVisible() == false)
					{
						mesh->SetVisiblity(true);
						UpdateLevelOfDetail(mesh, eye);
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

	void MeshVisualizer::UpdateLevelOfDetail(const SPtr<MeshComponent>& mesh, const Vec3& eye)
	{
		if (mesh->IsUseLevelOfDetail())
		{
			float distance = vec3::Length(eye - mesh->GetOwner().lock()->GetPosition());
			float size = mesh->GetMaxExtent() / distance;

			UINT lod = 0;
			if (size > 0.275f)
				lod = 0;
			else if (size <= 0.275f && size > 0.0875f)
				lod = 1;
			else if (size <= 0.0875f && size > 0.0175f)
				lod = 2;
			else
				lod = 3;
			mesh->SetLevelOfDetail(lod);
		}
		else
		{
			mesh->SetLevelOfDetail(0);
		}

		/*BOrientedBox obox = mesh->GetOrientedBox();
		Mat4 view = mat4::LookAt(eye, mesh->GetOwner().lock()->GetPosition(), up);
		obox.Transform(view);
		XMFLOAT3 corners[8];
		obox.GetBoundingOrientedBox().GetCorners(corners);
		Vec3 max_pos(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		Vec3 min_pos(FLT_MAX, FLT_MAX, FLT_MAX);
		for (const auto& corner : corners)
		{
			max_pos.x = max(max_pos.x, corner.x);
			max_pos.y = max(max_pos.y, corner.y);
			max_pos.z = max(max_pos.z, corner.z);
			min_pos.x = min(min_pos.x, corner.x);
			min_pos.y = min(min_pos.y, corner.y);
			min_pos.z = min(min_pos.z, corner.z);
		}
		Vec4 max4 = vec4::Transform(Vec4(max_pos), proj);
		Vec4 min4 = vec4::Transform(Vec4(min_pos), proj);
		max4 /= max4.w;
		min4 /= min4.w;*/

		//LOG_INFO(max(max4.x - min4.x, max4.y - min4.y) * 0.5f);
	}
}
