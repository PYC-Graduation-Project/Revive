#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Texture;

	enum class eUIState
	{
		kActive, kHide, kDead
	};

	class UserInterface : public IBaseObject
	{
	protected:
		UserInterface(const std::string& name);
		virtual ~UserInterface();

	public:
		virtual bool Initialize() override { return true; }
		virtual void Shutdown() override {}
		virtual void Update(float delta_time) override {}

	protected:
		std::string m_name;
		eUIState m_ui_state;
		Vec2 m_position;
		Vec2 m_size;
		bool m_is_updated_transform = true;
		SPtr<Texture> m_texture;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		eUIState GetUIState() const { return m_ui_state; }
		void SetUIState(eUIState state) { m_ui_state = state; }
		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position);
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size);
		bool IsUpdatedTransform() const { return m_is_updated_transform; }
		const SPtr<Texture> GetTexture() const { return m_texture; }
		void SetTexture(const SPtr<Texture>& texture);
	};
}

