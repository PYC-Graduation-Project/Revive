#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSCubeTextureIndexData
	{
		UINT index;
	};

	class SkyFrameResource
	{
	public:
		SkyFrameResource();
		~SkyFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSCubeTextureIndexData>> m_cube_texture_index_data;

	public:
		const UPtr<UploadBuffer<RSCubeTextureIndexData>>& GetCubeTextureIndexData() const { return m_cube_texture_index_data; }
	};
}



