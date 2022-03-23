#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSLightData
	{
		Vec3 light_color;
		float padding_0 = 0.0f;
		Vec3 light_direction;
	};

	class LightFrameResource
	{
	public:
		LightFrameResource();
		~LightFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSLightData>> m_light_data;
		UINT m_num_of_light = 1;

	public:
		const UPtr<UploadBuffer<RSLightData>>& GetLightData() const { return m_light_data; }
		UINT GetNumOfLight() const { return m_num_of_light; }
		void SetNumOfLight(UINT count) { m_num_of_light = count; }
	};
}



