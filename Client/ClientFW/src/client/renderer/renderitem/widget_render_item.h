#pragma once

namespace client_fw
{
	class WidgetComponent;

	template<class VerterType>
	class UploadPrimitive;
	class PivotWidgetVertex;
	class WorldWidgetVertex;
	enum class eWidgetSpaceType;

	class WidgetRenderItem final
	{
	public:
		WidgetRenderItem();
		virtual ~WidgetRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list, eWidgetSpaceType type);

		void RegisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);
		void UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);

	private:
		void UpdateWorldWidgets(ID3D12Device* device);
		void UpdateWidgets(ID3D12Device* device);

	private:
		bool m_is_need_world_widget_resource_create = false;
		bool m_is_need_widget_resource_create = false;
		UINT m_num_of_world_widget_ui_data = 0;
		UINT m_real_num_of_world_widget_ui_data = 0;
		UINT m_num_of_widget_ui_data = 0;
		UINT m_real_num_of_widget_ui_data = 0;

		UINT m_num_of_draw_world_widget_ui_data = 0;
		std::array<UINT, 3> m_num_of_draw_widget_ui_data = { 0, 0, 0 };
		std::array<UINT, 3> m_widget_start_vertex_locations = { 0, 0, 0 };

		UPtr<UploadPrimitive<WorldWidgetVertex>> m_world_widget_primitive;
		UPtr<UploadPrimitive<PivotWidgetVertex>> m_widget_primitive;

		std::vector<SPtr<WidgetComponent>> m_world_widget_components;
		std::vector<SPtr<WidgetComponent>> m_widget_components;

	public:
		bool IsDrawDataEmpty(eWidgetSpaceType type);
	};
}



