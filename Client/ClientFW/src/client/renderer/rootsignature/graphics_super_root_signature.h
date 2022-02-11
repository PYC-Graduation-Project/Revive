#pragma once
#include "client/renderer/rootsignature/root_signature.h"

namespace client_fw
{
	class CameraComponent;
	template<class T> class UploadBuffer;

	struct RSCameraData
	{
		Mat4 view_matrix;
		Mat4 projection_matrix;
		Vec3 camera_position;
	};

	class GraphicsSuperRootSignature final : public RootSignature
	{
	public:
		GraphicsSuperRootSignature();
		virtual ~GraphicsSuperRootSignature();

		virtual void Shutdown() override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) override;

		virtual bool CreateRootSignature(ID3D12Device* device) override;
		virtual void CreateResources(ID3D12Device* device) override;
		virtual void UpdateResources() override;

		void SetCameraResource(ID3D12GraphicsCommandList* command_list, const SPtr<CameraComponent>& camera_comp);

	private:
		UPtr<UploadBuffer<RSCameraData>> m_camera_data;
	};
}


