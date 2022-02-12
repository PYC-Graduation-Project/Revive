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

	bool operator<(const SplittingPlane& lhs, const SplittingPlane& rhs)
	{
		if (lhs.value == rhs.value)
			return ToUnderlying(lhs.info) < ToUnderlying(rhs.info);
		else
			return lhs.value < rhs.value;
	}

	namespace hyperplane_test
	{
		bool HyperplaneSeperation(const Vec3& n, const Vec3& v1,
			const Vec3& v2, const Vec3& v3, const Vec3& extents)
		{
			float p1 = vec3::Dot(n, v1);
			float p2 = vec3::Dot(n, v2);
			float p3 = vec3::Dot(n, v3);

			float min_value = min(p1, min(p2, p3));
			float max_value = max(p1, max(p2, p3));

			float ret = extents.x * fabs(n.x) +
				extents.y * fabs(n.y) + extents.z * fabs(n.z);

			return -ret > max_value || ret < min_value;
		}
	}

	KDTree::KDTree(UINT max_triangles, UINT max_dpeth)
		: m_max_triangles(max_triangles), m_max_depth(max_dpeth)
	{
	}

	void KDTree::Initialize(const BOrientedBox& box, std::vector<Triangle>& triangles)
	{
		CreateTree(nullptr, triangles, std::vector<size_t>(), box, 0);
#ifdef SHOW_TREE_INFO
		ShowTreeInfo();
#endif
		bool result = false;
		for (const auto& triangle : triangles)
		{
			if (triangle.count == 0)
				result = true;
		}
		if(result) LOG_WARN("경고!");
	}

	void KDTree::CreateTree(const SPtr<KDTreeNode>& parent_node, std::vector<Triangle>& triangles, 
		std::vector<size_t>&& tri_indices, const BOrientedBox& box, UINT depth)
	{
		SPtr<KDTreeNode> node = CreateSPtr<KDTreeNode>();
		node->triangle_indices = std::move(tri_indices);
		node->box = box;
		node->id = static_cast<UINT>(m_nodes.size());
		if (parent_node != nullptr)
		{
			node->parent_id = parent_node->id;
			if (parent_node->child0_id == -1)
				parent_node->child0_id = node->id;
			else if (parent_node->child1_id == -1 && parent_node->child0_id != node->id)
				parent_node->child1_id = node->id;
		}
		m_nodes.push_back(node);

		std::array<std::vector<SplittingPlane>, 3> splitting_planes;

		auto UpdateSpllitingPlanes([&splitting_planes](const Triangle& triangle, size_t index) {
			for (size_t i = 0; i < 3; ++i)
			{
				splitting_planes[i].emplace_back(SplittingPlane{ 
					ePrimitiveInfo::Start, min(triangle.v1[i], min(triangle.v2[i], triangle.v3[i])), index 
					});
				splitting_planes[i].emplace_back(SplittingPlane{
					ePrimitiveInfo::End, max(triangle.v1[i], max(triangle.v2[i], triangle.v3[i])), index 
					});
			}
			});
		
		if (depth == 0)
		{
			for (size_t index = 0; index < triangles.size(); ++index)
			{
				UpdateSpllitingPlanes(triangles[index], index);
				node->triangle_indices.emplace_back(index);
			}
		}
		else
		{
			for (size_t i = 0; i < node->triangle_indices.size(); ++i)
			{
				size_t index = node->triangle_indices[i];
				UpdateSpllitingPlanes(triangles[index], index);
			}
		}

		if (depth < m_max_depth && node->triangle_indices.size() > m_max_triangles)
		{
			for (size_t i = 0; i < 3; ++i)
				std::sort(splitting_planes[i].begin(), splitting_planes[i].end());

			float traversal_cost = 1.0f, intersection_cost = 1.0f;

			float best_cost = FLT_MAX, nosplit_cost = static_cast<float>(node->triangle_indices.size()) * intersection_cost;
			size_t split_index;
			float split_position;

			Vec3 center = node->box.GetCenter();
			Vec3 extents = node->box.GetExtents();

			size_t axis_index;
			float surface_area = 8.0f * (extents.x * extents.y + extents.x * extents.z + extents.y * extents.z);

			auto UpdateSplitInfo([node, center, surface_area, traversal_cost, intersection_cost, &axis_index, &split_index, &split_position, &best_cost]
			(const std::vector<SplittingPlane>& plane, size_t axis, Vec3 extents) {
					INT num_of_left = 0, num_of_right = static_cast<INT>(node->triangle_indices.size());
					float cost, left_surface_area, right_surface_area;
					float min_value = center[axis] - extents[axis], max_value = center[axis] + extents[axis];
					for (size_t index = 0; index < plane.size(); ++index)
					{
						const auto& point = plane[index];

						if (point.info == ePrimitiveInfo::End) --num_of_right;

						if (point.value > min_value && point.value < max_value)
						{
							extents[axis] = (point.value - min_value) * 0.5f;
							left_surface_area = 8.0f *
								(extents.x * extents.y + extents.x * extents.z + extents.y * extents.z) / surface_area;
							extents[axis] = (max_value - point.value) * 0.5f;
							right_surface_area = 8.0f *
								(extents.x * extents.y + extents.x * extents.z + extents.y * extents.z) / surface_area;

							cost = traversal_cost +
								intersection_cost * (left_surface_area * static_cast<float>(num_of_left) +
									right_surface_area * static_cast<float>(num_of_right));

							if (cost < best_cost)
							{
								best_cost = cost;
								axis_index = axis;
								split_index = index;
								split_position = point.value;
							}
						}

						if (point.info == ePrimitiveInfo::Start) ++num_of_left;
					}
	
					if (num_of_left != node->triangle_indices.size() || num_of_right != 0)
						LOG_WARN("Splliting Error");
				});

			for (size_t i = 0; i < 3; ++i)
				UpdateSplitInfo(splitting_planes[i], i, extents);
			

			if (best_cost < nosplit_cost)
			{
				Vec3 new_extents = extents;
				Vec3 new_center = center;

				std::vector<size_t> tri_indices;
				for (size_t i = 0; i <= split_index; ++i)
					if (splitting_planes[axis_index][i].info == ePrimitiveInfo::Start)
						tri_indices.push_back(splitting_planes[axis_index][i].tri_index);

				new_extents[axis_index] = (split_position - (center[axis_index] - extents[axis_index])) * 0.5f;
				new_center[axis_index] =  split_position - new_extents[axis_index];
				CreateTree(node, triangles, std::move(tri_indices), BOrientedBox(new_center, new_extents), depth + 1);

				for (size_t i = split_index + 1; i < splitting_planes[axis_index].size(); ++i)
					if (splitting_planes[axis_index][i].info == ePrimitiveInfo::End)
						tri_indices.push_back(splitting_planes[axis_index][i].tri_index);

				new_extents[axis_index] = ((center[axis_index] + extents[axis_index]) - split_position) * 0.5f;
				new_center[axis_index] = split_position + new_extents[axis_index];
				CreateTree(node, triangles, std::move(tri_indices), BOrientedBox(new_center, new_extents), depth + 1);
			}
		}

		if (node->child0_id == -1)
		{
			//Leaf Node는 AABB가 아닌 OBB를 활용하면 삼각형 검출을 줄일 수 있을 것으로 예상된다.
			if (node->triangle_indices.empty() == false)
			{
#ifdef SHOW_TREE_INFO
				UpdateTreeInfo(depth, static_cast<UINT>(node->triangle_indices.size()));
#endif
				std::vector<Vec3> positions;
				for (const auto& index : node->triangle_indices)
				{
					positions.push_back(triangles[index].v1);
					positions.push_back(triangles[index].v2);
					positions.push_back(triangles[index].v3);
					++triangles[index].count;
				}
				node->box = BOrientedBox(std::move(positions), false);
			}
		}

#ifdef SHOW_TREE_INFO
		++m_node_count;
#endif
	}
}
