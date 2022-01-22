#include "stdafx.h"
#include "client/util/octree/mesh_octree.h"
#include "client/object/component/mesh/core/mesh_component.h"
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
		if (mesh->GetOrientdBox().Intersects(m_root_node->bounding_box))
			RegisterMeshComponent(mesh, m_root_node);
		else
			m_out_of_range_mesh_comps.push_back(mesh);
		//mesh->SetVisiblity(false);
		mesh->SetVisiblity(true);
	}

	void MeshOctree::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kStatic)
		{
			for (const auto& tree_node : mesh->GetMeshTreeNodes())
			{
				//���������� Static Actor���� �̷��� Unregister�� �� �ʿ䰡 ������
				//��� ���� �ܰ迡���� �̷��� �����Ѵ�.

				auto node = tree_node.lock();

				auto iter = std::find(node->static_mesh_components.begin(),
					node->static_mesh_components.end(), mesh);
				std::iter_swap(iter, node->static_mesh_components.end() - 1);
				node->static_mesh_components.pop_back();
			}
		}
		else
		{
			for (const auto& tree_node : mesh->GetMeshTreeNodes())
			{
				auto node = tree_node.lock();

				auto iter = std::find(node->dynamic_mesh_components.begin(),
					node->dynamic_mesh_components.end(), mesh);
				std::iter_swap(iter, node->dynamic_mesh_components.end() - 1);
				node->dynamic_mesh_components.pop_back();
			}
		}
	}

	void MeshOctree::CreateChildNodeInfo(const SPtr<MeshTreeNode>& node_info, UINT depth)
	{
		Vec3 extents = Vec3(node_info->bounding_box.Extents) * 0.5f;

		for (UINT i = 0; i < 8; ++i)
		{
			node_info->child_nodes[i] = CreateSPtr<MeshTreeNode>();
			node_info->child_nodes[i]->parent_node = node_info;
			node_info->child_nodes[i]->bounding_box.Extents = extents;
			node_info->child_nodes[i]->bounding_box.Center.x =
				((i % 2 == 0) ? node_info->bounding_box.Center.x + extents.x
					: node_info->bounding_box.Center.x - extents.x);
			node_info->child_nodes[i]->bounding_box.Center.y =
				((i / 4 == 0) ? node_info->bounding_box.Center.y + extents.y
					: node_info->bounding_box.Center.y - extents.y);
			node_info->child_nodes[i]->bounding_box.Center.z =
				((i % 4 < 2) ? node_info->bounding_box.Center.z + extents.z
					: node_info->bounding_box.Center.z - extents.z);

			if (depth < m_depth)
				CreateChildNodeInfo(node_info->child_nodes[i], depth + 1);
		}
	}
	
	void MeshOctree::RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<MeshTreeNode>& node_info)
	{
		if (node_info->child_nodes[0] == nullptr)
		{
			if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kStatic)
			{
				node_info->static_mesh_components.push_back(mesh);
				//��� Static��ü���� Level�� Close�� ���� UnregisterEvent�� �߻��Ѵ�.
				//�׷��� �ѹ��� �� ����°� ȿ�����̴�. �� �κ��� ������ ���Ŀ� �� ���̰�
				//�ϴ��� Frustum ����� �� �۵��ϴ��� �ľ��ϱ� ���ؼ� TreeNode�� MeshComponent�� �߰��ϰڴ�.
				mesh->AddMeshTreeNode(node_info);
			}
			else
			{
				node_info->dynamic_mesh_components.push_back(mesh);
				mesh->AddMeshTreeNode(node_info);
			}
			return;
		}

		for (UINT i = 0; i < 8; ++i)
		{
			if (mesh->GetOrientdBox().Intersects(node_info->child_nodes[i]->bounding_box))
				RegisterMeshComponent(mesh, node_info->child_nodes[i]);
		}
	}
}
