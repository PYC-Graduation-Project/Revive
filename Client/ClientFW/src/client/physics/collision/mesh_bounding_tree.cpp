#include "stdafx.h"
#include "client/physics/collision/mesh_bounding_tree.h"

namespace client_fw
{
	StaticMeshBoundingTree::StaticMeshBoundingTree()
	{
		m_root_node = CreateSPtr<TriTreeNode>();
	}

	bool StaticMeshBoundingTree::Initialize(std::vector<Triangle>&& triangles)
	{
		if (m_root_node != nullptr)
		{
			Vec3 sum = Vec3(0.0f, 0.0f, 0.0f);
			for (const auto& tri : triangles)
			{
				sum += tri.v1;
				sum += tri.v2;
				sum += tri.v3;
			}
			CreateChildNode(m_root_node, std::move(triangles), std::move(sum), 0);
#ifdef SHOW_TREE_INFO
			ShowTreeInfo();
#endif // SHOW_TREE_INFO
			return true;
		}
		return false;
	}

	void StaticMeshBoundingTree::Update()
	{
	}
	void StaticMeshBoundingTree::CreateChildNode(const SPtr<TriTreeNode>& node, std::vector<Triangle>&& triangles, Vec3&& sum_of_positions, UINT depth)
	{
		Vec3 extents = node->box.GetExtents();
		Vec3 center = sum_of_positions / static_cast<float>((triangles.size() * 3));

		struct ExtentOrder
		{
			float extent;
			UINT xyz;
		};

		std::array<ExtentOrder, 3> order{ ExtentOrder{extents.x, 0}, {extents.y , 1}, {extents.z, 2} };

		std::sort(order.begin(), order.end(), [](ExtentOrder order1, ExtentOrder order2) {
			return order1.extent > order2.extent;
			});

		for (size_t i = 0; i < 3; ++i)
		{
			std::array<std::vector<Triangle>, 2> triangles_array;
			std::array<std::vector<Vec3>, 2> positions_array;
			UINT index = order[i].xyz;

			auto iter = std::partition(triangles.begin(), triangles.end(),
				[index, &center, &positions_array](const Triangle& tri) {
					if (1 < ((tri.v1[index] < center[index]) +
						(tri.v2[index] < center[index]) +
						(tri.v3[index] < center[index])))
					{
						positions_array[0].emplace_back(tri.v1);
						positions_array[0].emplace_back(tri.v2);
						positions_array[0].emplace_back(tri.v3);
						return true;
					}
					else
					{
						positions_array[1].emplace_back(tri.v1);
						positions_array[1].emplace_back(tri.v2);
						positions_array[1].emplace_back(tri.v3);
						return false;
					}
				});
			std::move(triangles.begin(), iter, std::back_inserter(triangles_array[0]));
			std::move(iter, triangles.end(), std::back_inserter(triangles_array[1]));

			if (triangles_array[0].size() == triangles.size() || triangles_array[1].size() == triangles.size())
				continue;

			std::array<Vec3, 2> sum_array;
			sum_array[0] = std::accumulate(positions_array[0].begin(), positions_array[0].end(), Vec3(0.0f, 0.0f, 0.0f));
			sum_array[1] = sum_of_positions - sum_array[0];

			for (size_t j = 0; j < 2; ++j)
			{
				node->child_nodes[j] = CreateSPtr<TriTreeNode>();
				node->child_nodes[j]->index = node->index * 2 + static_cast<UINT>(j + 1);
			
				if (triangles_array[j].size() < MIN_TRI_COUNT)
				{
#ifdef SHOW_TREE_INFO
					UpdateTreeInfo(depth + 1, static_cast<UINT>(triangles_array[j].size()));
#endif // SHOW_TREE_INFO
					node->child_nodes[j]->box = BOrientedBox(std::move(positions_array[j]), false);
					node->child_nodes[j]->triangles = std::move(triangles_array[j]);
				}
				else
				{
					node->child_nodes[j]->box = BOrientedBox(std::move(positions_array[j]));
					CreateChildNode(node->child_nodes[j], std::move(triangles_array[j]), std::move(sum_array[j]), depth + 1);
				}
			}
			return;
		}

#ifdef SHOW_TREE_INFO
		UpdateTreeInfo(depth, static_cast<UINT>(triangles.size()));
#endif // SHOW_TREE_INFO
		node->triangles = std::move(triangles);
	}
}
