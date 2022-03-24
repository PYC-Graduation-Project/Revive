#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class BillboardVertex;

	struct BillboardDrawInfo
	{
		UINT start_index;
		UINT num_of_draw_data;
		UINT fix_up_start_index;
		UINT num_of_draw_fix_up_data;
	};

	class BillboardFrameResource
	{
	public:
		BillboardFrameResource();
		~BillboardFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadPrimitive<BillboardVertex>> m_billboard_primitive;

		UINT m_size_of_billboard_primitive = 1;

		std::queue<BillboardDrawInfo> m_draw_info;

	public:
		const UPtr<UploadPrimitive<BillboardVertex>>& GetBillboardPrimitive() const { return m_billboard_primitive; }

		UINT GetSizeOfBillboardPrimitive() const { return m_size_of_billboard_primitive; }
		void SetSizeOfBillboardPrimitive(UINT value) { m_size_of_billboard_primitive = value; }

		void AddBillboardDrawInfo(BillboardDrawInfo&& info) { m_draw_info.emplace(std::move(info)); }

		BillboardDrawInfo GetBillboardDrawInfo();
	};
}



