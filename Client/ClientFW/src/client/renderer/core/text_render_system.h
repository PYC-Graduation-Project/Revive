#pragma once

#define __USE_DWRITE__

namespace client_fw
{
	struct Window;
	class RenderTextTexture;

	class TextRenderSystem
	{
	public:
		TextRenderSystem(const WPtr<Window>& window);
		~TextRenderSystem();

		TextRenderSystem(const TextRenderSystem&) = delete;
		TextRenderSystem& operator=(const TextRenderSystem&) = delete;

		bool Initialize(ID3D12Device* device, ID3D12CommandQueue* command_queue);
		void Shutdown();

		void Update(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list) const;

	private:
		bool Create11Device(ID3D12Device* device, ID3D12CommandQueue* command_queue);

	private:
		WPtr<Window> m_window;
		SPtr<RenderTextTexture> m_render_ui_texture;

		ComPtr<ID3D11DeviceContext> m_dx11_device_context;
		ComPtr<ID3D11On12Device> m_dx11_on_12_device;
		ComPtr<IDWriteFactory> m_write_factory;
		ComPtr<ID2D1Factory3> m_2d_factory;
		ComPtr<ID2D1Device2> m_2d_device;
		ComPtr<ID2D1DeviceContext2> m_2d_device_context;

		ComPtr<ID2D1SolidColorBrush> m_textBrush;
		ComPtr<IDWriteTextFormat> m_textFormat;

	public:
		ID3D12Resource* GetRenderUITexture() const;
	};
}




