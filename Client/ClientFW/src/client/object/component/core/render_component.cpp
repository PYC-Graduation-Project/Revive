#include "stdafx.h"
#include "client/object/component/core/render_component.h"
#include "client/renderer/core/render.h"
#include "client/util/octree/octree_manager.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	RenderComponent::RenderComponent(const std::string& name,
		int update_order, eRenderType type, const std::string& draw_shader_name)
		: SceneComponent(name, update_order)
		, m_type(type), m_draw_shader_name(draw_shader_name)
	{
	}

	bool RenderComponent::InitializeComponent()
	{
		bool ret = SceneComponent::InitializeComponent();
		RegisterToVisualOctree();
		ret = RegisterToRenderSystem();
		return ret;
	}

	void RenderComponent::ShutdownComponent()
	{
		UnregisterFromRenderSystem();
		UnregisterFromVisualOctree();
		SceneComponent::ShutdownComponent();
	}

	void RenderComponent::UpdateComponent(float delta_time)
	{
		SceneComponent::UpdateComponent(delta_time);
	}

	bool RenderComponent::RegisterToRenderSystem()
	{
		return Render::RegisterRenderComponent(SharedFromThis(), m_draw_shader_name);
	}

	void RenderComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterRenderComponent(SharedFromThis(), m_draw_shader_name);
	}

	void RenderComponent::RegisterToVisualOctree()
	{
		VisualOctreeManager::GetOctreeManager().RegisterRenderComponent(SharedFromThis());
	}

	void RenderComponent::UnregisterFromVisualOctree()
	{
		VisualOctreeManager::GetOctreeManager().UnregisterRenderComponent(SharedFromThis());
	}

	void RenderComponent::AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node)
	{
		m_visual_tree_node.push_back(tree_node);
	}

	SPtr<RenderComponent> RenderComponent::SharedFromThis()
	{
		return std::static_pointer_cast<RenderComponent>(shared_from_this());
	}
}
