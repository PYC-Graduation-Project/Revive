#pragma once

namespace client_fw
{
	class UserInterface;
	class UIPrimitive;

	class UIRenderItem final
	{
	public:
		UIRenderItem();
		virtual ~UIRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);

	private:
		UINT m_num_of_user_interface = 0;
		UPtr<UIPrimitive> m_ui_primitive;

	};
}



