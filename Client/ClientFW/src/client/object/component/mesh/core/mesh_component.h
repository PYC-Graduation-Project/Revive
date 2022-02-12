#pragma once
#include "client/object/component/core/scene_component.h"

namespace client_fw
{
	class Mesh;
	struct VisualTreeNode;

	class MeshComponent : public SceneComponent
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		virtual void UpdateOrientedBox() override;

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();
		void RegisterToMeshOctree();
		void UnregisterFromMeshOctree();

	protected:
		SPtr<Mesh> m_mesh;
		bool m_visibility = false;
		std::string m_draw_shader_name;

	public:
		virtual bool SetMesh(const std::string& file_path) = 0; 
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		bool IsVisible() const { return m_visibility; }
		void SetVisiblity(bool value) { m_visibility = value; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }

	private:
		UINT m_instance_index = 0;
		float m_max_extent = 0;
		UINT m_level_of_detail = 0;
		std::vector<WPtr<VisualTreeNode>> m_visual_tree_node;

	public:
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
		float GetMaxExtent() const { return m_max_extent; }
		bool IsUseLevelOfDetail() const;
		UINT GetLevelOfDetail() const { return m_level_of_detail; }
		void SetLevelOfDetail(UINT lod);
		void AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node);
		void ResetVisualTreeNode() { m_visual_tree_node.clear(); }
		const std::vector<WPtr<VisualTreeNode>>& GetVisualTreeNodes() const { return m_visual_tree_node; }

	protected:
		SPtr<MeshComponent> SharedFromThis();
	};
}


