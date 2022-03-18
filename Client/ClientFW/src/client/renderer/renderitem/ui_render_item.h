#pragma once

namespace client_fw
{
	class UserInterface;
	class CameraComponent;

	class UIVertex;

	class UIRenderItem final
	{
	public:
		UIRenderItem();
		virtual ~UIRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, const Vec2& window_size);
		void UpdateFrameResource(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list, std::function<void()>&& draw_function);

	private:
		UINT m_num_of_ui_data = 1;
		UINT m_num_of_draw_ui_data = 0;

		std::vector<UIVertex> m_vertices;
	};
}



