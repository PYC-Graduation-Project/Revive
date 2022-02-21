#pragma once

namespace client_fw
{
	class UserInterface;
	class UIPrimitive;
	class CameraComponent;

	class UIRenderItem final
	{
	public:
		UIRenderItem();
		virtual ~UIRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const SPtr<CameraComponent> main_camera);
		void Draw(ID3D12GraphicsCommandList* command_list);

	private:
		UINT m_size_of_user_interface_data = 0;
		UINT m_num_of_draw_ui_data = 0;
		UPtr<UIPrimitive> m_ui_primitive;

	};
}



