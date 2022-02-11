#pragma once
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
#define SHOW_TREE_INFO
#define MIN_TRI_COUNT 3

	struct Triangle
	{
		Vec3 v1, v2, v3;
		Vec3 n;
		UINT count = 0;
	};

	template <class Node>
	class MeshBoundingTree
	{
	public:
		virtual void Update() = 0;

	protected:
		SPtr<Node> m_root_node;
		bool m_is_need_update_world_matrix = false;

	public:
		const SPtr<Node>& GetRootNode() const { return m_root_node; }
		void NeedUpdateWorldMatrix() { m_is_need_update_world_matrix = true; }

#ifdef SHOW_TREE_INFO
	private:
		UINT m_count = 0;
		UINT m_min_count = UINT_MAX;
		UINT m_max_count = 0;
		UINT m_depth = 0;
		UINT m_min_depth = UINT_MAX;
		UINT m_max_depth = 0;
		UINT m_node_count = 0;

	protected:
		void ShowTreeInfo()
		{
			LOG_INFO("¿˙¿Â Node ºˆ : {0}", m_node_count);
			LOG_INFO("ªÔ∞¢«¸ ∆Ú±’ : {0}, √÷º“ : {1}, √÷¥Î : {2}", m_count / m_node_count, m_min_count, m_max_count);
			LOG_INFO("±Ì¿Ã ∆Ú±’ : {0}, √÷º“ : {1}, √÷¥Î : {2}", m_depth / m_node_count, m_min_depth, m_max_depth);
		}

		void UpdateTreeInfo(UINT depth, UINT count)
		{
			m_depth += depth;
			m_min_depth = min(m_min_depth, depth);
			m_max_depth = max(m_max_depth, depth);
			m_min_count = min(m_min_count, count);
			m_max_count = max(m_max_count, count);
			m_count += count;
			++m_node_count;
		}
#endif
	};

	struct TriTreeNode
	{
		UINT index = 0;
		BOrientedBox box;
		std::vector<Triangle> triangles;
		std::array<SPtr<TriTreeNode>, 2> child_nodes;
	};

	struct PosTriangle
	{

	};


	class StaticMeshBoundingTree : public MeshBoundingTree<TriTreeNode>
	{
	public:
		StaticMeshBoundingTree();

		bool Initialize(std::vector<Triangle>&& triangles);
		bool Initialize(std::vector<Vec3>&& positions);

		virtual	void Update() override;

	private:
		void CreateChildNode(const SPtr<TriTreeNode>& node,
			std::vector<Triangle>&& triangles, Vec3&& sum_of_positions, UINT depth);
	};


	//
	// The "KDTreeNode" code was written by referring to the site and book below.
	// https://www.keithlantz.net/2013/04/kd-tree-construction-using-the-surface-area-heuristic-stack-based-traversal-and-the-hyperplane-separation-theorem/
	// physics based rendering 4th
	//

	namespace hyperplane_test
	{
		inline bool HyperplaneSeperation(const Vec3& n, const Vec3& v1,
			const Vec3& v2, const Vec3& v3, const Vec3& extents);
	}

	enum class ePrimitiveInfo
	{
		End, Start
	};

	struct SplittingPlane
	{
		ePrimitiveInfo info;
		float value;
		size_t tri_index;
	};
	bool operator<(const SplittingPlane& lhs, const SplittingPlane& rhs);

	struct KDTreeNode
	{
		INT id = -1, parent_id = -1, child0_id = -1, child1_id = -1;
		BOrientedBox box;
		std::vector<size_t> triangle_indices;
	};

	class KDTree
	{
	public:
		KDTree(UINT max_triangles = 5, UINT max_dpeth = 20);

		void Initialize(const BOrientedBox& box, std::vector<Triangle>& triangles);

	private:
		void CreateTree(const SPtr<KDTreeNode>& node, std::vector<Triangle>& triangles, 
			std::vector<size_t>&& tri_indices, const BOrientedBox& box, UINT depth);

	private:
		UINT m_max_triangles;
		UINT m_max_depth;
		std::vector<SPtr<KDTreeNode>> m_nodes;

	public:
		const SPtr<KDTreeNode>& GetRootNode() const { return m_nodes.at(0); }
		const SPtr<KDTreeNode>& GetNode(size_t index) const { return m_nodes.at(index); }

#ifdef SHOW_TREE_INFO
	private:
		UINT m_count = 0;
		UINT m_min_count = UINT_MAX;
		UINT m_max_count = 0;
		UINT m_depth = 0;
		UINT m_min_depth = UINT_MAX;
		UINT m_real_max_depth = 0;
		UINT m_node_count = 0;
		UINT m_leaf_node_count = 0;

	protected:
		void ShowTreeInfo()
		{
			LOG_INFO("¿˙¿Â Node ºˆ : {0}, Leaf : {1}", m_node_count, m_leaf_node_count);
			LOG_INFO("ªÔ∞¢«¸ ∆Ú±’ : {0}, √÷º“ : {1}, √÷¥Î : {2}", m_count / m_leaf_node_count, m_min_count, m_max_count);
			LOG_INFO("±Ì¿Ã ∆Ú±’ : {0}, √÷º“ : {1}, √÷¥Î : {2}", m_depth / m_leaf_node_count, m_min_depth, m_real_max_depth);
		}

		void UpdateTreeInfo(UINT depth, UINT count)
		{
			m_depth += depth;
			m_min_depth = min(m_min_depth, depth);
			m_real_max_depth = max(m_real_max_depth, depth);
			m_min_count = min(m_min_count, count);
			m_max_count = max(m_max_count, count);
			m_count += count;
			++m_leaf_node_count;
		}
#endif
	};
}



