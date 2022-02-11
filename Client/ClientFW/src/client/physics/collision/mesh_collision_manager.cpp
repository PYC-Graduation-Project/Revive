#include "stdafx.h"
#include "client/physics/collision/mesh_collision_manager.h"
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/asset/mesh/vertex.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/object/actor/core/actor.h"


namespace client_fw
{
	MeshCollisionManager::MeshCollisionManager(const WPtr<MeshComponent>& mesh, eMeshCollisionType type)
		: m_mesh(mesh), m_type(type)
	{
	}

	StaticMeshCollisionManager::StaticMeshCollisionManager(
		const WPtr<MeshComponent>& mesh, const SPtr<KDTree>& tree, const SPtr<BOrientedBox>& box)
		: MeshCollisionManager(mesh, eMeshCollisionType::kStaticMesh)
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
			const auto& mesh1 = m_mesh.lock();
			const auto& mesh2 = mesh->m_mesh.lock();
			m_node_count = 0;
			m_tri_count = 0;
			switch (m_collision_info.complex)
			{
			case eCollisionComplex::kMedium:
			case eCollisionComplex::kHigh:
			{
				switch (mesh->m_collision_info.complex)
				{
				case eCollisionComplex::kMedium:
				case eCollisionComplex::kHigh:
				{
					BOrientedBox box2;
					box2.Transform(mesh->m_bounding_tree->GetRootNode()->box, mesh2->GetWorldMatrix());
				
					if (CheckCollision(mesh1->GetWorldMatrix(), m_bounding_tree, m_bounding_tree->GetRootNode(), mesh1,
						mesh2->GetWorldMatrix(), mesh->m_bounding_tree, mesh->m_bounding_tree->GetRootNode(), mesh2, box2))
					{
						LOG_INFO("{0} col {1}",	mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
					}
					break;
				}
				case eCollisionComplex::kLow:
				{
					if (CheckCollision(*m_box, mesh2->GetWorldMatrix(),
						mesh->m_bounding_tree, mesh->m_bounding_tree->GetRootNode()))
					{
						LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
					}
					break;
				}
				}
				break;
			}
			case eCollisionComplex::kLow:
			{
				switch (m_collision_info.complex)
				{
				case eCollisionComplex::kMedium:
				case eCollisionComplex::kHigh:
				{
					if (CheckCollision(*m_box, mesh2->GetWorldMatrix(),
						mesh->m_bounding_tree, mesh->m_bounding_tree->GetRootNode()))
					{
						LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
					}
					break;
				}
				case eCollisionComplex::kLow:
				{
					LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
					break;
				}
				}
				break;
			}
			}
			//LOG_INFO("충돌 검출 수 : {0} / {1}", m_node_count, m_tri_count);
		}
	}

	bool StaticMeshCollisionManager::CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<MeshComponent>& mesh1,
		const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<MeshComponent>& mesh2, const BOrientedBox& box2)
	{
		++m_node_count;
		BOrientedBox box1;
		box1.Transform(node1->box, mat1);

		ContainmentType type = box2.Contains(box1);
		switch (type)
		{
		case DirectX::DISJOINT:	return false;
		case DirectX::INTERSECTS:
			if (node1->child0_id == -1)
			{
				if (node2->child0_id == -1)
					return CheckTriangleCollision(mat2, mesh2, node2, mat1, mesh1, node1);
				else
				{
					if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child0_id), mesh2, mat1, tree1, node1, mesh1, box1))
						return true;
					if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child1_id), mesh2, mat1, tree1, node1, mesh1, box1))
						return true;
				}
			}
			else
			{
				if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child0_id), mesh1, mat2, tree2, node2, mesh2, box2))
					return true;
				if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child1_id), mesh1, mat2, tree2, node2, mesh2, box2))
					return true;
			}
			break;
		case DirectX::CONTAINS:
			if (node2->child0_id == -1)
			{
				//B가 A를 포함하는데 B가 leaf node인 경우는 이미 더이상 탐색 할 필요가 없다.
				return true;
				/*if (node1->child0_id == -1)
					return CheckTriangleCollision(mat1, mesh1, node1, mat2, mesh2, node2);
				else
				{
					if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child0_id), mesh1, mat2, tree2, node2, mesh2, box2))
						return true;
					if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child1_id), mesh1, mat2, tree2, node2, mesh2, box2))
						return true;
				}*/
			}
			else
			{
				if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child0_id), mesh2, mat1, tree1, node1, mesh1, box1))
					return true;
				if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child1_id), mesh2, mat1, tree1, node1, mesh1, box1))
					return true;
			}
			break;
		}

		return false;
	}

	bool StaticMeshCollisionManager::CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2)
	{
		return false;
	}

	bool StaticMeshCollisionManager::CheckTriangleCollision(const Mat4& mat1, const SPtr<MeshComponent>& mesh1, const SPtr<KDTreeNode>& node1,
		const Mat4& mat2, const SPtr<MeshComponent>& mesh2, const SPtr<KDTreeNode>& node2)
	{
		//둘다 Complex가 High인 경우만 검출한다. 둘중 하나가 Medium인 경우는 Box끼리의 검출만 한다.
		if (mesh1->GetCollisionManager()->GetCollisionInfo().complex == eCollisionComplex::kHigh &&
			mesh2->GetCollisionManager()->GetCollisionInfo().complex == eCollisionComplex::kHigh)
		{
			if (node1->triangle_indices.empty() == false && node2->triangle_indices.empty() == false)
			{
				auto vertices1 = reinterpret_cast<TextureLightVertex*>(mesh1->GetMesh()->GetVertexBufferBlob(0)->GetBufferPointer());
				auto vertices2 = reinterpret_cast<TextureLightVertex*>(mesh2->GetMesh()->GetVertexBufferBlob(0)->GetBufferPointer());

				for (size_t i1 : node1->triangle_indices)
				{
					Vec3 v1(vertices1[i1 * 3 + 0].GetPosition());
					Vec3 v2(vertices1[i1 * 3 + 1].GetPosition());
					Vec3 v3(vertices1[i1 * 3 + 2].GetPosition());
					v1.TransformCoord(mat1), v2.TransformCoord(mat1), v3.TransformCoord(mat1);

					for (size_t i2 : node2->triangle_indices)
					{
						Vec3 v4(vertices2[i2 * 3 + 0].GetPosition());
						Vec3 v5(vertices2[i2 * 3 + 1].GetPosition());
						Vec3 v6(vertices2[i2 * 3 + 2].GetPosition());
						v4.TransformCoord(mat2), v5.TransformCoord(mat2), v6.TransformCoord(mat2);

						++m_tri_count;
						if (triangle_test::Intersect(v1, v2, v3, v4, v5, v6))
						{
							return true;
						}
					}
				}
			}
			return false;
		}
		return true;
	}

}
