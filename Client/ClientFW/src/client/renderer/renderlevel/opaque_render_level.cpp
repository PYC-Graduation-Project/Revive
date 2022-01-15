#include "stdafx.h"
#include "client/renderer/renderlevel/opaque_render_level.h"

namespace client_fw
{
	OpaqueRenderLevel::OpaqueRenderLevel(const std::string& name)
		: GraphicsRenderLevel(name, { DXGI_FORMAT_R8G8B8A8_UNORM }, DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
	}

	bool OpaqueRenderLevel::CreateRootSignature(ID3D12Device* device)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 2> root_parameters;
		root_parameters[0].InitAsShaderResourceView(0, 0);
		root_parameters[1].InitAsConstantBufferView(0, 0);

		CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc(static_cast<UINT>(root_parameters.size()), root_parameters.data(),
			0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature_blob = nullptr;
		ComPtr<ID3DBlob> error_blob = nullptr;

		if (FAILED(D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
			signature_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			return false;
		}

		if (error_blob != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(error_blob->GetBufferPointer()));
		}

		return (SUCCEEDED(device->CreateRootSignature(0, signature_blob->GetBufferPointer(),
			signature_blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature))));
	}

	void OpaqueRenderLevel::UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
	}

	void OpaqueRenderLevel::SetRootCommonResource(ID3D12GraphicsCommandList* command_list)
	{
	}
}
