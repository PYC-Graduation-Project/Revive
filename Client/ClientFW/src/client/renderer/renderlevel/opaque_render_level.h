#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class OpaqueRenderLevel final : public GraphicsRenderLevel
	{
	public:
		OpaqueRenderLevel(eRenderLevelType level_type, const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~OpaqueRenderLevel() = default;

	protected:
		//virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override final;
		//virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) override final;
	};
}


