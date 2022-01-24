#pragma once
#include <DirectXCollision.h>

namespace client_fw
{
	class MeshComponent;
	class CameraComponent;

	struct MeshTreeNode
	{
		BoundingBox bounding_box;
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

		void SetVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		void CreateChildNodeInfo(const SPtr<MeshTreeNode>& node, UINT depth);
		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh, const SPtr<MeshTreeNode>& node);
		void SetVisibilityFromCamera(const BoundingFrustum& bounding_frustum, ContainmentType type, const SPtr<MeshTreeNode>& node);

	private:
		float m_width;
		Vec3 m_position;
		UINT m_depth = 1;
		SPtr<MeshTreeNode> m_root_node;
		std::vector<SPtr<MeshComponent>> m_out_of_range_mesh_comps;
		std::vector<SPtr<MeshComponent>> m_movable_mesh_comps;

	};
}



