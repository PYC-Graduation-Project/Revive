#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSShadowTextureData
	{
		Mat4 uv_from_ndc_matrix;
		Vec2 inverse_texture_size;
		UINT shadow_texture_index;
	};

	class ShadowFrameResource
	{
	public:
		ShadowFrameResource();
		~ShadowFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSShadowTextureData>> m_shadow_texture_data;
		UINT m_size_of_shadow_texture = 1;

	public:
		const UPtr<UploadBuffer<RSShadowTextureData>>& GetShadowTextureData() const { return m_shadow_texture_data; }
		UINT GetSizeOfShadowTexture() const { return m_size_of_shadow_texture; }
		void SetSizeOfShadowTexture(UINT count) { m_size_of_shadow_texture = count; }
	};
}



