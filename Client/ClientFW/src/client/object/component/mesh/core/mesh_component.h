#pragma once
#include "client/object/component/core/component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class Mesh;
	struct VisualTreeNode;

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
		BOrientedBox m_oriented_box;
		float m_max_extent = 0;
		UINT m_level_of_detail = 0;
		std::vector<WPtr<VisualTreeNode>> m_visual_tree_node;
		bool m_visibility = false;
		UINT m_instance_index = 0;
		std::string m_draw_shader_name;

	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		virtual void SetMesh(const std::string& file_path);
		const BOrientedBox& GetOrientedBox() const { return m_oriented_box; }
		float GetMaxExtent() const { return m_max_extent; }
		bool IsUseLevelOfDetail() const;
		UINT GetLevelOfDetail() const { return m_level_of_detail; }
		void SetLevelOfDetail(UINT lod);
		void AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node);
		const std::vector<WPtr<VisualTreeNode>>& GetVisualTreeNodes() const { return m_visual_tree_node; }
		bool IsVisible() const { return m_visibility; }
		void SetVisiblity(bool value) { m_visibility = value; }
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }
	};
}


