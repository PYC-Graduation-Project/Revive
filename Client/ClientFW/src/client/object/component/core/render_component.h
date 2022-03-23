#pragma once
#include "client/object/component/core/scene_component.h"

namespace client_fw
{
	struct VisualTreeNode;

	enum class eRenderType
	{
		kMesh, kShape, kBillboard, kWidget
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
		bool m_hidden_in_game = false;
		bool m_is_register_render_system = true;
		UINT m_render_item_index = 0;
		std::string m_draw_shader_name;
		std::vector<WPtr<VisualTreeNode>> m_visual_tree_node;

	public:
		eRenderType GetRenderType() const { return m_type; }
		bool IsVisible() const { return m_visibility; }
		void SetVisiblity(bool value) { m_visibility = value; }
		bool IsHiddenInGame() const { return m_hidden_in_game; }
		void SetHiddenInGame(bool value) { m_hidden_in_game = value; }
		UINT GetRenderItemIndex() const { return m_render_item_index; }
		void SetRenderItemIndex(UINT index) { m_render_item_index = index; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }
		void AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node);
		void ResetVisualTreeNode() { m_visual_tree_node.clear(); }
		const std::vector<WPtr<VisualTreeNode>>& GetVisualTreeNodes() const { return m_visual_tree_node; }

	protected:
		SPtr<RenderComponent> SharedFromThis();
	};
}


