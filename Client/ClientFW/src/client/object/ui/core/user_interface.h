#pragma once
namespace client_fw
{
	class Texture;

	class UITexture
	{
	public:
		UITexture(const Vec2& position, const Vec2& size, const SPtr<Texture>& texture,
			const Vec2& coord = vec2::ZERO, const Vec2& tilling = Vec2(1.f, 1.f))
			: m_position(position), m_size(size)
			, m_texture(texture), m_coordinate(coord), m_tilling(tilling)
		{}

	private:
		Vec2 m_position;
		Vec2 m_size;
		Vec2 m_coordinate;
		Vec2 m_tilling;
		SPtr<Texture> m_texture;

	public:
		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coordinate) { m_coordinate = coordinate; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
	};

	class UserInterface : public std::enable_shared_from_this<UserInterface>
	{
	protected:
		UserInterface(const std::string& name, size_t num_of_visible_texture);
		virtual ~UserInterface();

	public:
		virtual bool Initialize() { return true; }
		virtual void Shutdown() {}

	protected:
		std::string m_name;
		bool m_is_visible;
		Vec2 m_position;
		Vec2 m_size;
		bool m_is_hovered = false;

	private:
		std::vector<SPtr<UITexture>> m_visible_textures;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		bool IsVisible() const { return m_is_visible; }
		void SetVisible(bool state) { m_is_visible = state; }
		const Vec2& GetPosition() const { return m_position; }
		virtual void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		virtual void SetSize(const Vec2& size) { m_size = size; }

		UINT GetNumOfVisibleTexture() const { return static_cast<UINT>(m_visible_textures.size()); }
		const std::vector<SPtr<UITexture>> GetVisibleTextures() const { return m_visible_textures; }

		bool IsHovered() const;

	protected:
		void SetVisibleTexture(const SPtr<UITexture>& texture, UINT index);
	};
}

