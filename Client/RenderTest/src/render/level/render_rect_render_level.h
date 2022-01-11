#pragma once
#include <client/renderer/renderlevel/core/render_level.h>

namespace client_fw
{
	class RenderRectRenderLevel final : public GraphicsRenderLevel
	{
	public:
		RenderRectRenderLevel(const std::string& name);
		virtual ~RenderRectRenderLevel() = default;

	protected:
		virtual bool CreateRootSignature(ID3D12Device* device) override final;
		virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override final;
		virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) override final;
	};
}


