#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/core/text_render_system.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/render_resource_manager.h"

namespace client_fw
{
	TextRenderSystem::TextRenderSystem(const WPtr<Window>& window)
		: m_window(window)
	{
#ifdef __USE_DWRITE__
		const auto& w = window.lock();
		m_render_ui_texture = CreateSPtr<RenderTextTexture>(IVec2(w->width, w->height));
#endif
	}

	TextRenderSystem::~TextRenderSystem()
	{
	}

	bool TextRenderSystem::Initialize(ID3D12Device* device, ID3D12CommandQueue* command_queue)
	{
#ifdef __USE_DWRITE__
		if (Create11Device(device, command_queue) == false)
		{
			LOG_ERROR("Could not create DX11 device");
			return false;
		}

		if (m_render_ui_texture->Initialize(device) == false ||
			m_render_ui_texture->Initialize2D(m_dx11_on_12_device.Get(), m_2d_device_context.Get()) == false)
		{
			LOG_ERROR("Could not create render ui texture");
			return false;
		}

		RenderResourceManager::GetRenderResourceManager().RegisterTexture(m_render_ui_texture);

		m_2d_device_context->SetTarget(m_render_ui_texture->Get2DRenderTarget());

		if (FAILED((m_2d_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textBrush))))
			return false;
		if (FAILED(m_write_factory->CreateTextFormat(
			L"±Ã¼­Ã¼",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50,
			L"ko-kr",
			&m_textFormat
		))) return false;
			
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
#endif
		return true;
	}

	bool TextRenderSystem::Create11Device(ID3D12Device* device, ID3D12CommandQueue* command_queue)
	{
		UINT dx11_device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS d2d_factory_options = {};

#if defined(_DEBUG)
		{
			dx11_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
			d2d_factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		}
#endif

		ComPtr<ID3D11Device> dx11_device;
		if (FAILED(D3D11On12CreateDevice(device, dx11_device_flags,
			nullptr, 0, reinterpret_cast<IUnknown**>(&command_queue),
			1, 0, &dx11_device, &m_dx11_device_context, nullptr)))
		{
			LOG_ERROR("Could not create 11On12 device");
			return false;
		}

		if (FAILED(dx11_device.As(&m_dx11_on_12_device)))
			return false;

		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3),
			&d2d_factory_options, &m_2d_factory)))
		{
			LOG_ERROR("Could not create 2d factory");
			return false;
		}

		ComPtr<IDXGIDevice> dxgi_device;
		if (FAILED(m_dx11_on_12_device.As(&dxgi_device)))
			return false;
		if (FAILED(m_2d_factory->CreateDevice(dxgi_device.Get(), &m_2d_device)))
			return false;
		if (FAILED(m_2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_2d_device_context)))
			return false;
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_write_factory)))
			return false;

		return true;
	}

	ID3D12Resource* TextRenderSystem::GetRenderUITexture() const
	{
		return m_render_ui_texture->GetResource();
	}

	void TextRenderSystem::Shutdown()
	{
	}

	void TextRenderSystem::Update(ID3D12Device* device)
	{
	}

	void TextRenderSystem::Draw(ID3D12GraphicsCommandList* command_list) const
	{
#ifdef __USE_DWRITE__
		ID3D11Resource* resources[] = { m_render_ui_texture->GetWrappedRenderTarget().Get() };

		m_dx11_on_12_device->AcquireWrappedResources(resources, _countof(resources));

		D2D1_SIZE_F rtSize = m_render_ui_texture->Get2DRenderTarget()->GetSize();
		D2D1_RECT_F textRect = D2D1::RectF(0, 0, rtSize.width, rtSize.height);
		WCHAR text[] = L"´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ";

		m_2d_device_context->BeginDraw();
		m_2d_device_context->Clear();
		//m_2d_device_context->SetTransform(D2D1::Matrix3x2F::Identity());
		m_2d_device_context->DrawText(
			text,
			_countof(text) - 1,
			m_textFormat.Get(),
			&textRect,
			m_textBrush.Get()
		);

		m_2d_device_context->EndDraw();


		m_dx11_on_12_device->ReleaseWrappedResources(resources, _countof(resources));

		m_dx11_device_context->Flush();
#endif
	}
}
