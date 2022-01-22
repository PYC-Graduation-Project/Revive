#pragma once
#include <DirectXCollision.h>

namespace client_fw
{
	class MeshComponent;

	struct MeshTreeNode
	{
		BoundingBox bounding_box;
		std::vector<SPtr<MeshComponent>> static_mesh_components;
		std::vector<SPtr<MeshComponent>> dynamic_mesh_components;
		std::array<SPtr<MeshTreeNode>, 8> child_nodes;
		WPtr<MeshTreeNode> parent_node;
	};

	class MeshOctree
	{
	public:
		MeshOctree(float width, Vec3 pos = vec3::ZERO, UINT depth = 4);
		
		void Initialize();
		void Shutdown();
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh);

	private:
		void CreateChildNodeInfo(const SPtr<MeshTreeNode>& node_info, UINT depth);
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<MeshTreeNode>& node_info);

	private:
		float m_width;
		Vec3 m_position;
		UINT m_depth = 1;
		SPtr<MeshTreeNode> m_root_node;
		std::vector<SPtr<MeshComponent>> m_out_of_range_mesh_comps;

	};
}



