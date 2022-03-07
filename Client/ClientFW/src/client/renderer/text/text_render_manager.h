#pragma once

namespace client_fw
{
	struct TextFormat
	{
		std::wstring name;
		UINT font_size;
		DWRITE_FONT_STYLE font_style;
	};

	bool operator<(const TextFormat& t1, const TextFormat& t2);

	class TextRenderManager final
	{
	public:
		TextRenderManager();
		~TextRenderManager();

		TextRenderManager(const TextRenderManager&) = delete;
		TextRenderManager& operator=(const TextRenderManager&) = delete;

		bool Initialize(IDWriteFactory* factory, ID2D1DeviceContext2* context);
		void Shutdown();

		void Update();
		void Draw(ID2D1DeviceContext2* context);

	private:
		std::map<Vec4, ComPtr<ID2D1SolidColorBrush>> m_text_brushes;
		std::map<TextFormat, ComPtr<IDWriteTextFormat>> m_text_formats;


	public:
		ComPtr<ID2D1SolidColorBrush> LoadTextBrush(ID2D1DeviceContext2* context, const Vec4& color);
		ComPtr<IDWriteTextFormat> LoadTextFormat(IDWriteFactory* factory, const TextFormat& format);
	};
}


