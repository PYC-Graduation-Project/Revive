#pragma once

namespace client_fw
{
	class BillboardComponent;
	class TextureBillboardComponent;
	class MaterialBillboardComponent;
	class BillboardPrimitive;

	class BillboardRenderItem
	{
	public:
		BillboardRenderItem();
		virtual ~BillboardRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) {}
		virtual void Draw(ID3D12GraphicsCommandList* command_list, bool is_fix_up);
		//void DrawFixUp(ID3D12GraphicsCommandList* command_list);

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) {}
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) {}

	protected:
		bool m_is_need_resource_create = false;
	
		UINT m_num_of_billboard_data = 0;
		std::array<UINT, 2> m_num_of_draw_billboard_data;
		std::array<UINT, 2> m_start_vertex_locations;
		UPtr<BillboardPrimitive> m_billboard_primitive;

	public:
		bool IsDrawDataEmpty(bool is_fix_up) { return m_num_of_draw_billboard_data[static_cast<UINT>(is_fix_up)] == 0; }
	};
	
	class TextureBillboardRenderItem final : public BillboardRenderItem
	{
	public:
		TextureBillboardRenderItem();
		virtual ~TextureBillboardRenderItem();

		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) override;

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;

	private:
		std::vector<SPtr<TextureBillboardComponent>> m_billboard_components;
	};

	class MaterialBillboardRenderItem final : public BillboardRenderItem
	{
	public:
		MaterialBillboardRenderItem();
		virtual ~MaterialBillboardRenderItem();

		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) override;

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;

	private:
		std::vector<SPtr<MaterialBillboardComponent>> m_billboard_components;
	};
}



