#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class ShadowRenderLevel final : public GraphicsRenderLevel
	{
	public:
		ShadowRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~ShadowRenderLevel() = default;
	};

	class ShadowCubeRenderLevel final : public GraphicsRenderLevel
	{
	public:
		ShadowCubeRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~ShadowCubeRenderLevel() = default;
	};
}


