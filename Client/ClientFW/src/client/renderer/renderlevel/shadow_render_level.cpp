#include "stdafx.h"
#include "client/renderer/renderlevel/shadow_render_level.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	ShadowRenderLevel::ShadowRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kShadow, root_signature,
			{}, DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
	}
}
