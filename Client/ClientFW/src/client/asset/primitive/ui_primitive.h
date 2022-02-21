#pragma once
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	class UIVertex;

	class UIPrimitive : public Primitive
	{
	public:
		UIPrimitive();
		virtual ~UIPrimitive() = default;

		virtual bool Initialize(ID3D12Device * device, ID3D12GraphicsCommandList * command_list) override;
		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device * device, UINT num_of_data);
		virtual void UpdateUIVertices(const std::vector<UIVertex>& vertices);
		virtual void Draw(ID3D12GraphicsCommandList * command_list, UINT lod = 0) const override;

	protected:
		SPtr<UploadVertexInfo> m_vertex_info;
		UINT m_num_of_vertex = 0;
	};
}



