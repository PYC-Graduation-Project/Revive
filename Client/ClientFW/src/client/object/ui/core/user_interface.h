#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Texture;

	enum class eUIState
	{
		kActive, kHide, kDead
	};

	class UITexture
	{
	public:
		UITexture(const Vec2& position, const Vec2& size, const SPtr<Texture>& texture)
			: m_position(position), m_size(size), m_texture(texture)
		{}

	private:
		Vec2 m_position;
		Vec2 m_size;
		SPtr<Texture> m_texture;

	public:
		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
	};

	class UserInterface : public IBaseObject
	{
	protected:
		UserInterface(const std::string& name, size_t num_of_visible_texture);
		virtual ~UserInterface();

	public:
		virtual bool Initialize() override { return true; }
		virtual void Shutdown() override {}
		virtual void Update(float delta_time) override;

	protected:
		std::string m_name;
		eUIState m_ui_state;
		Vec2 m_position;
		Vec2 m_size;

	private:
		std::vector<SPtr<UITexture>> m_visible_textures;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		eUIState GetUIState() const { return m_ui_state; }
		void SetUIState(eUIState state) { m_ui_state = state; }
		const Vec2& GetPosition() const { return m_position; }
		virtual void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		virtual void SetSize(const Vec2& size) { m_size = size; }

		UINT GetNumOfVisibleTexture() const { return static_cast<UINT>(m_visible_textures.size()); }
		const std::vector<SPtr<UITexture>> GetVisibleTextures() const { return m_visible_textures; }

	protected:
		void SetVisibleTexture(const SPtr<UITexture>& texture, UINT index);
	};
}

