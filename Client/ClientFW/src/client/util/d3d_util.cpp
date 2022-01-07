#include "stdafx.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	UINT D3DUtil::s_cbvsrvuav_descirptor_increment_size = 0;
	UINT D3DUtil::s_rtv_descirptor_increment_size = 0;
	UINT D3DUtil::s_dsv_descirptor_increment_size = 0;

	bool D3DUtil::s_is_use_4x_mass = false; 
	UINT D3DUtil::s_4x_msaa_quality = 0;
}