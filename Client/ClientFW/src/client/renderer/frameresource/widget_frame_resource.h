#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class WorldWidgetVertex;
	class PivotWidgetVertex;

	struct WorldWidgetDrawInfo
	{
		UINT start_index;
		UINT num_of_draw_data;
	};

	struct PivotWidgetDrawInfo
	{
		UINT billboard_start_index;
		UINT num_of_draw_billboard_data;
		UINT fix_up_start_index;
		UINT num_of_draw_fix_up_data;
	};

	class WidgetFrameResource
	{
	public:
		WidgetFrameResource();
		~WidgetFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadPrimitive<WorldWidgetVertex>> m_world_widget_primitive;
		UPtr<UploadPrimitive<PivotWidgetVertex>> m_pivot_widget_primitive;

		std::queue<WorldWidgetDrawInfo> m_world_widget_draw_info;
		std::queue<PivotWidgetDrawInfo> m_pivot_widget_draw_info;

	public:
		const UPtr<UploadPrimitive<WorldWidgetVertex>>& GetWorldWidgetPrimitive() const { return m_world_widget_primitive; }
		const UPtr<UploadPrimitive<PivotWidgetVertex>>& GetPivotWidgetPrimitive() const { return m_pivot_widget_primitive; }

		void AddWorldWidgetDrawInfo(WorldWidgetDrawInfo&& info) { m_world_widget_draw_info.emplace(std::move(info)); }
		void AddPivotWidgetDrawInfo(PivotWidgetDrawInfo&& info) { m_pivot_widget_draw_info.emplace(std::move(info)); }

		WorldWidgetDrawInfo GetWorldWidgetDrawInfo();
		PivotWidgetDrawInfo GetPivotWidgetDrawInfo();
	};

}

