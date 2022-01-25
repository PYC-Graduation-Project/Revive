#pragma once
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class MeshComponent;
	class CameraComponent;

	struct MeshTreeNode
	{
		BBox bounding_box;
		std::vector<SPtr<MeshComponent>> mesh_components;
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
		void CreateChildNodeInfo(const SPtr<MeshTreeNode>& node, UINT depth);
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<MeshTreeNode>& node);\

	private:
		float m_width;
		Vec3 m_position;
		UINT m_depth = 1;
		SPtr<MeshTreeNode> m_root_node;
		std::vector<SPtr<MeshComponent>> m_out_of_range_mesh_comps;
		std::vector<SPtr<MeshComponent>> m_movable_mesh_comps;


	public:
		const SPtr<MeshTreeNode>& GetRootNode() const { return m_root_node; }
		const std::vector<SPtr<MeshComponent>> GetOutOfRangeMeshes() const { return m_out_of_range_mesh_comps; }
		const std::vector<SPtr<MeshComponent>> GetMovableMeshes() const { return m_movable_mesh_comps; }
	};
}



