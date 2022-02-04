#pragma once
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class MeshComponent;
	class CameraComponent;

	struct VisualTreeNode
	{
		BBox bounding_box;
		std::vector<SPtr<MeshComponent>> mesh_components;
		std::array<SPtr<VisualTreeNode>, 8> child_nodes;
		WPtr<VisualTreeNode> parent_node;
	};

	class VisualOctree
	{
	public:
		VisualOctree(float width, Vec3 pos = vec3::ZERO, UINT depth = 3);
		
		void Initialize();
		void Shutdown();
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);

	private:
		void CreateChildNodeInfo(const SPtr<VisualTreeNode>& node, UINT depth);
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<VisualTreeNode>& node);

	private:
		float m_width;
		Vec3 m_position;
		UINT m_depth;
		SPtr<VisualTreeNode> m_root_node;

	public:
		const SPtr<VisualTreeNode>& GetRootNode() const { return m_root_node; }
		const std::vector<SPtr<MeshComponent>> GetOutOfRangeMeshes() const { return m_root_node->mesh_components; }
	};
}



