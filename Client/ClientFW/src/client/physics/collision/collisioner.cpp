#include "stdafx.h"
#include "client/physics/collision/collisioner.h"
#include "client/object/component/core/scene_component.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/asset/primitive/vertex.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	Collisioner::Collisioner(const WPtr<SceneComponent>& owner, eMeshCollisionType type)
		: m_owner(owner), m_type(type)
	{
	}

	StaticMeshCollisioner::StaticMeshCollisioner(const WPtr<StaticMeshComponent>& owner)
		: Collisioner(owner, eMeshCollisionType::kStaticMesh)
	{
	}

	bool StaticMeshCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case client_fw::eMeshCollisionType::kStaticMesh:
		{
			const auto& mesh1 = std::static_pointer_cast<StaticMeshComponent>(GetOwner());
			const auto& mesh2 = std::static_pointer_cast<StaticMeshComponent>(other->GetCollisioner()->GetOwner());
			CheckCollsionWithStaticMesh(mesh1, mesh1->GetCollisioner()->GetCollisionInfo().complex, 
				mesh2, mesh2->GetCollisioner()->GetCollisionInfo().complex);
			break;
		}
		default:
			break;
		}
		return false;
	}

	void StaticMeshCollisioner::CheckCollsionWithStaticMesh(const SPtr<StaticMeshComponent>& mesh1, eCollisionComplex complex1, 
		const SPtr<StaticMeshComponent>& mesh2, eCollisionComplex complex2)
	{
		if (mesh1->GetOrientedBox()->Intersects(*mesh2->GetOrientedBox()))
		{
			const auto& tree1 = mesh1->GetStaticMesh()->GetBoundingTree();
			const auto& tree2 = mesh2->GetStaticMesh()->GetBoundingTree();

			m_node_count = 0;
			m_tri_count = 0;
			switch (complex1)
			{
			case eCollisionComplex::kMedium:
			case eCollisionComplex::kHigh:
			{
				switch (complex2)
				{
				case eCollisionComplex::kMedium:
				case eCollisionComplex::kHigh:
				{
					BOrientedBox box2;
					box2.Transform(tree2->GetRootNode()->box, mesh2->GetWorldMatrix());

					if (CheckCollision(mesh1->GetWorldMatrix(), tree1, tree1->GetRootNode(), mesh1,
						mesh2->GetWorldMatrix(), tree2, tree2->GetRootNode(), mesh2, box2))
					{
						LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
					}
					break;
				}
				case eCollisionComplex::kLow:
				{
					if (CheckCollision(*mesh2->GetOrientedBox(), mesh1->GetWorldMatrix(), tree1, tree1->GetRootNode()))
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
				switch (complex2)
				{
				case eCollisionComplex::kMedium:
				case eCollisionComplex::kHigh:
				{
					if (CheckCollision(*mesh1->GetOrientedBox(), mesh2->GetWorldMatrix(), tree2, tree2->GetRootNode()))
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
			//LOG_INFO("�浹 ���� �� : {0} / {1}", m_node_count, m_tri_count);
		}
	}

	bool StaticMeshCollisioner::CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<StaticMeshComponent>& mesh1,
		const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<StaticMeshComponent>& mesh2, const BOrientedBox& box2)
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
				//B�� A�� �����ϴµ� B�� leaf node�� ���� �̹� ���̻� Ž�� �� �ʿ䰡 ����.
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

	bool StaticMeshCollisioner::CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2)
	{
		return false;
	}

	bool StaticMeshCollisioner::CheckTriangleCollision(const Mat4& mat1, const SPtr<StaticMeshComponent>& mesh1, const SPtr<KDTreeNode>& node1, const Mat4& mat2,
		const SPtr<StaticMeshComponent>& mesh2, const SPtr<KDTreeNode>& node2)
	{
		//�Ѵ� Complex�� High�� ��츸 �����Ѵ�. ���� �ϳ��� Medium�� ���� Box������ ���⸸ �Ѵ�.
		if (mesh1->GetCollisioner()->GetCollisionInfo().complex == eCollisionComplex::kHigh &&
			mesh2->GetCollisioner()->GetCollisionInfo().complex == eCollisionComplex::kHigh)
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
