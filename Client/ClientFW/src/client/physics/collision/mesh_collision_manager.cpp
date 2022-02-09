#include "stdafx.h"
#include "client/physics/collision/mesh_collision_manager.h"
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	MeshCollisionManager::MeshCollisionManager(eMeshCollisionType type)
		: m_type(type)
	{
	}

	StaticMeshCollisionManager::StaticMeshCollisionManager(
		const SPtr<StaticMeshBoundingTree>& tree, const SPtr<BOrientedBox>& box)
		: MeshCollisionManager(eMeshCollisionType::kStaticMesh)
		, m_bounding_tree(tree), m_box(box)
	{
	}

	bool StaticMeshCollisionManager::CheckCollisionWithOtherMesh(const SPtr<MeshCollisionManager>& mesh)
	{
		switch (mesh->GetMeshCollisionType())
		{

		case client_fw::eMeshCollisionType::kStaticMesh:
			CheckCollision(std::reinterpret_pointer_cast<StaticMeshCollisionManager>(mesh));
			break;
		default:
			break;
		}

		return false;
	}

	void StaticMeshCollisionManager::CheckCollision(const SPtr<StaticMeshCollisionManager>& mesh)
	{
		if (m_box->Intersects(*mesh->GetOrientedBox()))
		{
			m_count = 0;
			//LOG_INFO("½ÃÀÛ {0} to {1}", m_owner.lock()->GetName(), mesh->GetOwner().lock()->GetName());
			for (const auto& node : m_bounding_tree->GetRootNode()->child_nodes)
			{
				if (CheckCollision(m_owner.lock()->GetWorldMatrix(), node,
					mesh->GetOwner().lock()->GetWorldMatrix(), mesh->m_bounding_tree->GetRootNode(),
					*mesh->GetOrientedBox()))
				{
					LOG_INFO("{0} col {1}",
						m_owner.lock()->GetName(),
						mesh->GetOwner().lock()->GetName());
					break;
				}
			}
			//LOG_INFO("¼ö : {0}", m_count);
		
		}
	}

	bool StaticMeshCollisionManager::CheckCollision(const Mat4& mat1, const SPtr<TriTreeNode>& node1,
		const Mat4& mat2, const SPtr<TriTreeNode>& node2, const BOrientedBox& box2)
	{
		++m_count;
		BOrientedBox box1;
		box1.Transform(node1->box, mat1);

		ContainmentType type = box2.Contains(box1);
		switch (type)
		{
		case DirectX::DISJOINT:	return false;
		case DirectX::INTERSECTS:
			if (node1->child_nodes[0] == nullptr)
			{
				if (node2->child_nodes[0] == nullptr)
					return CheckTriangleCollision(mat2, node2, mat1, node1);
				else
				{
					for (const auto& node : node2->child_nodes)
					{
						if (CheckCollision(mat2, node, mat1, node1, box1))
							return true;
					}
				}
			}
			else
			{
				for (const auto& node : node1->child_nodes)
				{
					if (CheckCollision(mat1, node, mat2, node2, box2))
						return true;
				}
			}
			break;
		case DirectX::CONTAINS:
			if (node2->child_nodes[0] == nullptr)
			{
				if (node1->child_nodes[0] == nullptr)
					return CheckTriangleCollision(mat1, node1, mat2, node2);
				else
				{
					for (const auto& node : node1->child_nodes)
					{
						if (CheckCollision(mat1, node, mat2, node2, box2))
							return true;
					}
				}
			}
			else
			{
				for (const auto& node : node2->child_nodes)
				{
					if (CheckCollision(mat2, node, mat1, node1, box1))
						return true;
				}
			}
			break;
		}

		return false;
	}

	bool StaticMeshCollisionManager::CheckTriangleCollision(const Mat4& mat1, const SPtr<TriTreeNode>& node1, const Mat4& mat2, const SPtr<TriTreeNode>& node2)
	{
		
		//LOG_TRACE("Index : {0} - {1}, Index : {2} - {3}", node1->index, node1->triangles.size(), node2->index, node2->triangles.size());
		//return true;
		for (const auto& tri1 : node1->triangles)
		{
			Triangle new_tri1 = tri1;
			new_tri1.v1.TransformCoord(mat1);
			new_tri1.v2.TransformCoord(mat1);
			new_tri1.v3.TransformCoord(mat1);

			for (const auto& tri2 : node2->triangles)
			{
				Triangle new_tri2 = tri2;
				new_tri2.v1.TransformCoord(mat2);
				new_tri2.v2.TransformCoord(mat2);
				new_tri2.v3.TransformCoord(mat2);

			/*	LOG_TRACE("{0}, {1}, {2}", new_tri1.v1, new_tri1.v2, new_tri1.v3);
				LOG_TRACE("{0}, {1}, {2}", new_tri2.v1, new_tri2.v2, new_tri2.v3);*/

				if (triangle_test::Intersect(new_tri1.v1, new_tri1.v2, new_tri1.v3,
					new_tri2.v1, new_tri2.v2, new_tri2.v3))
					return true;
			}
		}
		return false;
	}

}
