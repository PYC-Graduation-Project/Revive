#pragma once

namespace client_fw
{
	class WidgetComponent;

	class PivotWidgetVertex;
	class WorldWidgetVertex;
	enum class eWidgetSpaceType;

	class WidgetRenderItem final
	{
	public:
		WidgetRenderItem(const std::string& owner_shader_name);
		virtual ~WidgetRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device);
		void UpdateFrameResource(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list, 
			std::function<void()>&& world_function, std::function<void()>&& billboard_function,
			std::function<void()>&& fix_up_function);

		void RegisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);
		void UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);

	private:
		void UpdateWorldWidgets(ID3D12Device* device);
		void UpdatePivotWidgets(ID3D12Device* device);

	private:
		std::string m_owner_shader_name;

		std::vector<WorldWidgetVertex> m_world_widget_vertices;
		std::vector<PivotWidgetVertex> m_pivot_widget_vertices;

		std::vector<SPtr<WidgetComponent>> m_world_widget_components;
		std::vector<SPtr<WidgetComponent>> m_widget_components;
	};
}



