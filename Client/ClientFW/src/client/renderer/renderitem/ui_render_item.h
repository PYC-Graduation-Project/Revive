#pragma once

namespace client_fw
{
	class UserInterface;
	class CameraComponent;

	template<class VertexType>
	class UploadPrimitive;
	class UIVertex;

	class UIRenderItem final
	{
	public:
		UIRenderItem();
		virtual ~UIRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, const Vec2& window_size);
		void Draw(ID3D12GraphicsCommandList* command_list);

	private:
		UINT m_num_of_ui_layer = 0;
		UINT m_size_of_user_interface_data = 0;
		UINT m_num_of_draw_ui_data = 0;
		UPtr<UploadPrimitive<UIVertex>> m_ui_primitive;

	public:
		bool IsDrawDataEmpty() { return m_num_of_draw_ui_data == 0; }
	};
}



