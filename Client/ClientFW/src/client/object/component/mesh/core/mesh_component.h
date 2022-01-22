#pragma once
#include <DirectXCollision.h>
#include "client/object/component/core/component.h"

namespace client_fw
{
	class Mesh;
	struct MeshTreeNode;

	class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent>
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void UpdateWorldMatrix() override;

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();
		void RegisterToMeshOctree();
		void UnregisterFromMeshOctree();

	protected:
		SPtr<Mesh> m_mesh;
		BoundingOrientedBox m_oriented_box;
		std::vector<WPtr<MeshTreeNode>> m_mesh_tree_node;
		bool m_visibility = true;
		UINT m_instance_index = 0;
		std::string m_draw_shader_name;


	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		virtual void SetMesh(const std::string& file_path);
		const BoundingOrientedBox& GetOrientdBox() const { return m_oriented_box; }
		void AddMeshTreeNode(const WPtr<MeshTreeNode>& tree_node);
		const std::vector<WPtr<MeshTreeNode>>& GetMeshTreeNodes() const { return m_mesh_tree_node; }
		bool IsVisible() const { return m_visibility; }
		void SetVisiblity(bool value) { m_visibility = value; }
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }
	};
}


