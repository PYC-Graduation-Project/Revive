#pragma once

namespace client_fw
{
	class BillboardComponent;
	class TextureBillboardComponent;
	class MaterialBillboardComponent;
	class BillboardPrimitive;

	enum class eBillboardRenderType
	{
		kTexture, kFixUpTexture,
		kMaterial, kFixUpMaterial
	};

	class BillboardRenderItem final
	{
	public:
		BillboardRenderItem();
		virtual ~BillboardRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void PreDraw(ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list, eBillboardRenderType type);
		//void DrawFixUp(ID3D12GraphicsCommandList* command_list);

		void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp);
		void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp);

	private:
		bool m_is_need_resource_create = false;
		std::vector<SPtr<TextureBillboardComponent>> m_texture_billboard_components;
		std::vector<SPtr<MaterialBillboardComponent>> m_material_billboard_components;

		UINT m_num_of_billboard_data = 0;
		std::array<UINT, 4> m_num_of_draw_billboard_data;
		std::array<UINT, 4> m_start_vertex_locations;
		UPtr<BillboardPrimitive> m_billboard_primitive;

	public:
		bool IsDrawDataEmpty(eBillboardRenderType type) const { return m_num_of_draw_billboard_data[ToUnderlying(type)] == 0; }
	};
}



