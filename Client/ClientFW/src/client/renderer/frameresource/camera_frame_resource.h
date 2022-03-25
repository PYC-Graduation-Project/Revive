#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSCameraData
	{
		Mat4 view_matrix;
		Mat4 projection_matrix;
		Mat4 view_projection_matrix;
		Mat4 inverse_view_matrix;
		Vec4 perspective_values;
		Vec3 camera_position;
		UINT final_texture_index;
		XMUINT4 gbuffer_texture_indices;
		UINT num_of_directional_light;
	};

	class CameraFrameResource
	{
	public:
		CameraFrameResource();
		~CameraFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSCameraData>> m_camera_data;
		UINT m_size_of_camera = 0;

	public:
		const UPtr<UploadBuffer<RSCameraData>>& GetCameraData() const { return m_camera_data; }
		UINT GetSizeOfCamera() const { return m_size_of_camera; }
		void SetSizeOfCamera(UINT count) { m_size_of_camera = count; }

	};
}



