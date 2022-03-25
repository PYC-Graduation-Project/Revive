#pragma once
#include "client/object/component/core/scene_component.h"

namespace client_fw
{
	struct VisualTreeNode;

	enum class eRenderType
	{
		kMesh, kShape, kBillboard, kWidget
	};

	// RenderSystem에 등록되어서 그려지는 것들을 표현한 Component
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

	protected:
		virtual bool RegisterToRenderSystem();
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
		// SetVisibility 함수는 Frustum culling을 통해서 camera에 대해 그려지는지 여부를 주는 변수입니다.
		// 따라서 사용자가 이 함수를 호출해도 rendering에 원하는 대로 영향을 줄 수 없습니다.
		void SetVisiblity(bool value) { m_visibility = value; }

		bool IsHiddenInGame() const { return m_hidden_in_game; }
		// 사용자가 이 RenderComponent를 화면에 그리고 싶지 않다면 이 함수를 호출하면 됩니다.
		void SetHiddenInGame(bool value) { m_hidden_in_game = value; }

		//render item index는 이 render component가 등록된 render item에서의 index위치를 알려줍니다.
		//삭제할 때 find등으로 찾지 말고 바로 삭제하기 위한 변수
		UINT GetRenderItemIndex() const { return m_render_item_index; }
		void SetRenderItemIndex(UINT index) { m_render_item_index = index; }

		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }

		// Frustum culling에 필요한 Octree에서 어떤 Node에 있는지 추가하는 함수입니다.
		// 삭제 시 빠르게 삭제하기 위한 기능입니다. 사용자가 따로 호출할 필요는 없는 함수입니다. 
		void AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node);
		void ResetVisualTreeNode() { m_visual_tree_node.clear(); }
		const std::vector<WPtr<VisualTreeNode>>& GetVisualTreeNodes() const { return m_visual_tree_node; }

	protected:
		SPtr<RenderComponent> SharedFromThis();
	};
}


