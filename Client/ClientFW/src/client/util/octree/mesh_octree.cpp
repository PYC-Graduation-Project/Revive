#include "stdafx.h"
#include "client/util/octree/mesh_octree.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	MeshOctree::MeshOctree(float width, Vec3 pos, UINT depth)
		: m_width(width), m_position(pos), m_depth(depth)
	{
		m_root_node = CreateSPtr<MeshTreeNode>();
	}

	void MeshOctree::Initialize()
	{
		m_root_node->bounding_box = BBox(m_position, Vec3(m_width * 0.5f, m_width * 0.5f, m_width * 0.5f));
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
			if (mesh->GetOrientedBox().Intersects(m_root_node->bounding_box))
				RegisterMeshComponent(mesh, m_root_node);
			else
				m_out_of_range_mesh_comps.push_back(mesh);
		}
		mesh->SetVisiblity(false);
	}

	void MeshOctree::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		auto UnregisterMesh([&mesh](std::vector<SPtr<MeshComponent>>& meshes) {
			auto iter = std::find(meshes.begin(), meshes.end(), mesh);
			if (iter != meshes.end())
			{
				std::iter_swap(iter, meshes.end() - 1);
				meshes.pop_back();
			}
			});

		if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
		{
			UnregisterMesh(m_movable_mesh_comps);
		}
		else
		{
			if (mesh->GetMeshTreeNodes().empty())
			{
				UnregisterMesh(m_out_of_range_mesh_comps);
			}
			else
			{
				for (const auto& tree_node : mesh->GetMeshTreeNodes())
					UnregisterMesh(tree_node.lock()->mesh_components);
			}
		}
	}

	void MeshOctree::CreateChildNodeInfo(const SPtr<MeshTreeNode>& node, UINT depth)
	{
		Vec3 extents = node->bounding_box.GetExtents() * 0.5f;
		Vec3 center = node->bounding_box.GetCenter();

		for (UINT i = 0; i < 8; ++i)
		{
			node->child_nodes[i] = CreateSPtr<MeshTreeNode>();
			node->child_nodes[i]->parent_node = node;
			node->child_nodes[i]->bounding_box.SetExtents(extents);
			Vec3 new_center;
			new_center.x = ((i % 2 == 0) ? center.x + extents.x	: center.x - extents.x);
			new_center.y = ((i / 4 == 0) ? center.y + extents.y	: center.y - extents.y);
			new_center.z = ((i % 4 < 2) ? center.z + extents.z : center.z - extents.z);
			node->child_nodes[i]->bounding_box.SetCenter(new_center);
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
			if (mesh->GetOrientedBox().Intersects(node->child_nodes[i]->bounding_box))
				RegisterMeshComponent(mesh, node->child_nodes[i]);
		}
	}
}
