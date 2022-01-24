#include "stdafx.h"
#include "client/util/octree/mesh_octree.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	MeshOctree::MeshOctree(float width, Vec3 pos, UINT depth)
		: m_width(width), m_position(pos), m_depth(depth)
	{
		m_root_node = CreateSPtr<MeshTreeNode>();
	}

	void MeshOctree::Initialize()
	{
		m_root_node->bounding_box.Center = m_position;
		m_root_node->bounding_box.Extents = Vec3(m_width * 0.5f, m_width * 0.5f, m_width * 0.5f);
		CreateChildNodeInfo(m_root_node, 1);
	}

	void MeshOctree::Shutdown()
	{
		m_root_node = nullptr;
	}

	void MeshOctree::RegisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
		{
			m_movable_mesh_comps.push_back(mesh);
		}
		else
		{
			if (mesh->GetOrientdBox().Intersects(m_root_node->bounding_box))
				RegisterMeshComponent(mesh, m_root_node);
			else
				m_out_of_range_mesh_comps.push_back(mesh);
		}
		mesh->SetVisiblity(false);
	}

	void MeshOctree::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		auto UnregisterMesh([](std::vector<SPtr<MeshComponent>>& meshes, const SPtr<MeshComponent>& mesh) {
			auto iter = std::find(meshes.begin(), meshes.end(), mesh);
			std::iter_swap(iter, meshes.end() - 1);
			meshes.pop_back();
			});

		if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
		{
			UnregisterMesh(m_movable_mesh_comps, mesh);
		}
		else
		{
			if (mesh->GetMeshTreeNodes().empty())
			{
				UnregisterMesh(m_out_of_range_mesh_comps, mesh);
			}
			else
			{
				for (const auto& tree_node : mesh->GetMeshTreeNodes())
					UnregisterMesh(tree_node.lock()->mesh_components, mesh);
			}
		}
	}

	void MeshOctree::SetVisibilityFromCamera(const SPtr<CameraComponent>& camera)
	{
		const auto& bounding_frustum = camera->GetBoudingFrustum();

		ContainmentType type = bounding_frustum.Contains(m_root_node->bounding_box);
 		if (type != ContainmentType::DISJOINT)
		{
			SetVisibilityFromCamera(bounding_frustum, type, m_root_node);
		}

		for (const auto& mesh : m_out_of_range_mesh_comps)
		{
			if (bounding_frustum.Intersects(mesh->GetOrientdBox()))
				mesh->SetVisiblity(true);
		}

		for (const auto& mesh : m_movable_mesh_comps)
		{
			if (bounding_frustum.Intersects(mesh->GetOrientdBox()))
				mesh->SetVisiblity(true);
		}
	}


	void MeshOctree::CreateChildNodeInfo(const SPtr<MeshTreeNode>& node, UINT depth)
	{
		Vec3 extents = Vec3(node->bounding_box.Extents) * 0.5f;

		for (UINT i = 0; i < 8; ++i)
		{
			node->child_nodes[i] = CreateSPtr<MeshTreeNode>();
			node->child_nodes[i]->parent_node = node;
			node->child_nodes[i]->bounding_box.Extents = extents;
			node->child_nodes[i]->bounding_box.Center.x =
				((i % 2 == 0) ? node->bounding_box.Center.x + extents.x
					: node->bounding_box.Center.x - extents.x);
			node->child_nodes[i]->bounding_box.Center.y =
				((i / 4 == 0) ? node->bounding_box.Center.y + extents.y
					: node->bounding_box.Center.y - extents.y);
			node->child_nodes[i]->bounding_box.Center.z =
				((i % 4 < 2) ? node->bounding_box.Center.z + extents.z
					: node->bounding_box.Center.z - extents.z);

			if (depth < m_depth)
				CreateChildNodeInfo(node->child_nodes[i], depth + 1);
		}
	}
	
	void MeshOctree::RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<MeshTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			node->mesh_components.push_back(mesh);
			mesh->AddMeshTreeNode(node);
			return;
		}

		for (UINT i = 0; i < 8; ++i)
		{
			if (mesh->GetOrientdBox().Intersects(node->child_nodes[i]->bounding_box))
				RegisterMeshComponent(mesh, node->child_nodes[i]);
		}
	}

	void MeshOctree::SetVisibilityFromCamera(const BoundingFrustum& bounding_frustum, ContainmentType type, const SPtr<MeshTreeNode>& node)
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
					SetVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i]);
			}
			break;
		}
		case DirectX::CONTAINS:
			for (UINT i = 0; i < 8; ++i)
				SetVisibilityFromCamera(bounding_frustum, type, node->child_nodes[i]);
			break;
		default:
			break;
		}
		
	}
}
