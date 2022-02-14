#pragma once
#include "client/object/component/core/scene_component.h"

namespace client_fw
{
	struct VisualTreeNode;

	enum class eRenderType
	{
		kMesh,
	};

	class RenderComponent : public SceneComponent
	{
	protected:
		RenderComponent(const std::string& name, int update_order,
			eRenderType type, const std::string& draw_shader_name);
		virtual ~RenderComponent() = default;

		virtual bool InitializeComponent() override final;
		virtual void ShutdownComponent() override final;
		virtual void UpdateComponent(float delta_time) override final;

	public:
		virtual void UpdateLevelOfDetail(const Vec3& eye) {}

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();
		void RegisterToVisualOctree();
		void UnregisterFromVisualOctree();

	protected:
		eRenderType m_type;
		bool m_visibility = false;
		std::string m_draw_shader_name;
		std::vector<WPtr<VisualTreeNode>> m_visual_tree_node;

	public:
		eRenderType GetRenderType() const { return m_type; }
		bool IsVisible() const { return m_visibility; }
		void SetVisiblity(bool value) { m_visibility = value; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }
		void AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node);
		void ResetVisualTreeNode() { m_visual_tree_node.clear(); }
		const std::vector<WPtr<VisualTreeNode>>& GetVisualTreeNodes() const { return m_visual_tree_node; }

	protected:
		SPtr<RenderComponent> SharedFromThis();
	};
}


