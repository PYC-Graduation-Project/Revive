#pragma once

namespace client_fw
{
	class D3DUtil final
	{
	public:
		static UINT s_cbvsrvuav_descirptor_increment_size;
		static UINT s_rtv_descirptor_increment_size;
		static UINT s_dsv_descirptor_increment_size;

	public:
		static inline void SetObjectName(ID3D12Object* object, std::string name)
		{
#ifdef _DEBUG
			object->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.data());
#endif
		}

		static inline void SetObjectName(IDXGIObject* object, std::string name)
		{
#ifdef _DEBUG
			object->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.data());
#endif
		}
	};
}

