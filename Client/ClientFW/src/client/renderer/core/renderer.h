#pragma once

namespace client_fw
{
	struct Window;

	class RenderSystem;

	class Renderer final
	{
	public:
		Renderer(const WPtr<Window>& window);
		~Renderer();

		bool Initialize();
		void Shutdown();
		bool Render();

		bool UpdateViewport();

	private:
		WPtr<Window> m_window;
		UPtr<RenderSystem> m_render_system;

	private:
		bool CreateDevice();
		bool Create11Device();
		bool CreateCommandObjects();
		bool CreateSwapchain();

		bool CreateRtvDsvHeaps();
		bool CreateRenderTargetViews();
		bool CreateDepthStencilView();
		bool ResizeViewport();

		void SetViewAndScissor(float left, float top, float width, float height);
		void WaitForGpuCompelete();
		void MoveToNextFrame();

		bool InitializeRenderSystem();

	private:
		ComPtr<IDXGIFactory4> m_factory = nullptr;
		ComPtr<IDXGISwapChain3> m_swap_chain = nullptr;
		ComPtr<ID3D12Device> m_device = nullptr;

		static constexpr UINT s_swap_chain_buffer_count = 2;
		UINT m_cur_swapchain_buffer = 0;

		ComPtr<ID3D12Fence> m_fence = nullptr;
		std::array<UINT64, s_swap_chain_buffer_count> m_fence_values;
		HANDLE m_fence_event = NULL;

		bool m_is_use_4x_mass = false;
		UINT m_4x_msaa_quality = 0;

		ComPtr<ID3D12CommandQueue> m_command_queue = nullptr;
		ComPtr<ID3D12CommandAllocator> m_command_allocator = nullptr;
		ComPtr<ID3D12GraphicsCommandList> m_command_list = nullptr;

		ComPtr<ID3D11DeviceContext> m_dx11_device_context;
		ComPtr<ID3D11On12Device> m_dx11_on_12_device;
		ComPtr<IDWriteFactory> m_write_factory;
		ComPtr<ID2D1Factory3> m_2d_factory;
		ComPtr<ID2D1Device2> m_2d_device;
		ComPtr<ID2D1DeviceContext2> m_2d_device_context;

		std::array<ComPtr<ID3D12Resource>, s_swap_chain_buffer_count> m_rtv_buffers;
		std::array<ComPtr<ID3D11Resource>, s_swap_chain_buffer_count> m_wrapped_back_buffers;
		std::array<ComPtr<ID2D1Bitmap1>, s_swap_chain_buffer_count> m_2d_render_targets;
		std::array<D3D12_CPU_DESCRIPTOR_HANDLE, s_swap_chain_buffer_count> m_rtv_cpu_handles;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap = nullptr;

		ComPtr<ID3D12Resource> m_dsv_buffers = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handles;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap = nullptr;

		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissor_rect;

		DXGI_FORMAT m_rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		UINT m_dx11_device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS m_d2d_factory_options;

		ComPtr<ID2D1SolidColorBrush> m_textBrush;
		ComPtr<IDWriteTextFormat> m_textFormat;

	public:
		const ComPtr<ID3D12Device>& GetDevice() const { return m_device; }
		const ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const { return m_command_list; }
		const ComPtr<IDXGISwapChain3>& GetSwapChain() const { return m_swap_chain; }
		const ComPtr<ID3D12Resource>& GetCurrentRenderTarget() const { return m_rtv_buffers[m_cur_swapchain_buffer]; }
	};
}


