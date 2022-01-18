#pragma once
#include "client/renderer/rootsignature/root_signature.h"

namespace client_fw
{
	class CameraComponent;

	struct RSCameraData
	{
		Mat4 view_matrix;
		Mat4 projection_matrix;
		Vec3 camera_position;
	};

	struct RSMaterialData
	{
		Vec4 base_color;
	};

	class GraphicsSuperRootSignature final : public RootSignature
	{
	public:
		GraphicsSuperRootSignature() = default;

		virtual void Draw(ID3D12GraphicsCommandList* command_list) override;

		virtual bool CreateRootSignature(ID3D12Device* device) override;
		virtual void CreateResources(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void UpdateResources() override;

		void SetCameraResource(ID3D12GraphicsCommandList* command_list, const SPtr<CameraComponent>& camera_comp);

	private:
		ComPtr<ID3D12Resource> m_camera_data;
		BYTE* m_camera_mapped_data;

		ComPtr<ID3D12Resource> m_material_data;
		BYTE* m_material_mapped_data;
	};
}


