#pragma once
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	class BillboardVertex;

	class BillboardPrimitive : public Primitive
	{
	public:
		BillboardPrimitive();
		virtual ~BillboardPrimitive() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device* device, UINT num_of_data);
		virtual void UpdateBillboardVertices(const std::vector<BillboardVertex>& vertices);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_vertex) const override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_vertex, UINT start_vertex_location) const;

	protected:
		SPtr<UploadVertexInfo> m_vertex_info;
	};


}


