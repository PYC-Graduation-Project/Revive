#include "stdafx.h"
#include "client/renderer/text/text_render_manager.h"

namespace client_fw
{
	bool operator<(const TextFormat& t1, const TextFormat& t2)
	{
		if (t1.name == t2.name)
		{
			if (t1.font_size == t2.font_size)
				return t1.font_style < t2.font_style;
			else
				return t1.font_size < t2.font_size;
		}
		else
			return t1.name < t2.name;
	}

	TextRenderManager::TextRenderManager()
	{
	}

	TextRenderManager::~TextRenderManager()
	{
	}

	bool TextRenderManager::Initialize(IDWriteFactory* factory, ID2D1DeviceContext2* context)
	{
		LoadTextBrush(context, Vec4(1.f, 1.f, 1.f, 1.f));
		LoadTextFormat(factory, TextFormat{ L"±Ã¼­Ã¼", 100, DWRITE_FONT_STYLE_NORMAL });

		return true;
	}

	void TextRenderManager::Shutdown()
	{
	}

	void TextRenderManager::Update()
	{
	}

	void TextRenderManager::Draw(ID2D1DeviceContext2* context)
	{
		D2D1_RECT_F textRect = D2D1::RectF(0, 0, 1366, 768);
		WCHAR text[] = L"´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ";

		context->DrawText(
			text,
			_countof(text) - 1,
			m_text_formats[TextFormat{ L"±Ã¼­Ã¼", 100, DWRITE_FONT_STYLE_NORMAL }].Get(),
			&textRect,
			m_text_brushes[(Vec4(1.f, 1.f, 1.f, 1.f))].Get()
		);
	}

	ComPtr<ID2D1SolidColorBrush> TextRenderManager::LoadTextBrush(ID2D1DeviceContext2* context, const Vec4& color)
	{
		ComPtr<ID2D1SolidColorBrush> brush;
		auto iter = m_text_brushes.find(color);
		if (iter != m_text_brushes.cend())
			return iter->second.Get();
		else
		{
			if (FAILED((context->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, color.w), &brush))))
			{
				LOG_ERROR("Could not create solid brush [{0}]", color);
				return false;
			}
			else
			{
				m_text_brushes[color] = brush;
				return brush;
			}
		}
	}

	ComPtr<IDWriteTextFormat> TextRenderManager::LoadTextFormat(IDWriteFactory* factory, const TextFormat& format)
	{
		ComPtr<IDWriteTextFormat> text_format;
		auto iter = m_text_formats.find(format);
		if (iter != m_text_formats.cend())
			return iter->second.Get();
		else
		{
			if (FAILED(factory->CreateTextFormat(format.name.c_str(), NULL,
				DWRITE_FONT_WEIGHT_THIN, format.font_style, DWRITE_FONT_STRETCH_NORMAL,
				format.font_size, L"ko-kr", &text_format)))
			{
				LOG_ERROR(L"Could not create text format [{0}, {1}]", format.name, format.font_size);
				return false;
			}
			else
			{
				m_text_formats[format] = text_format;
				text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				return text_format;
			}
		}
	}
}
