#include "stdafx.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"
#include "client/util/d3d_util.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/util/camera_component.h"


namespace client_fw
{
	void GraphicsSuperRootSignature::Draw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->SetGraphicsRootSignature(m_root_signature.Get());
	}

	bool GraphicsSuperRootSignature::CreateRootSignature(ID3D12Device* device)
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

		return SUCCEEDED(device->CreateRootSignature(0, signature_blob->GetBufferPointer(),
			signature_blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)));
	}

	void GraphicsSuperRootSignature::CreateResources(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		UINT byte_size = static_cast<UINT>((sizeof(RSCameraData) + 255) & ~255);
		m_camera_data = D3DUtil::CreateUploadBuffer(device, command_list, byte_size, &m_camera_mapped_data);
	}

	void GraphicsSuperRootSignature::UpdateResources()
	{
	}

	void GraphicsSuperRootSignature::SetCameraResource(ID3D12GraphicsCommandList* command_list, const SPtr<CameraComponent>& camera_comp)
	{
		RSCameraData camera_data;
		camera_data.view_matrix = mat4::Transpose(camera_comp->GetViewMatrix());
		camera_data.projection_matrix = mat4::Transpose(camera_comp->GetProjectionMatrix());
		camera_data.camera_position = camera_comp->GetOwner().lock()->GetPosition();

		memcpy(m_camera_mapped_data, &camera_data, sizeof(RSCameraData));

		command_list->SetGraphicsRootConstantBufferView(1, m_camera_data->GetGPUVirtualAddress());
	}
}
