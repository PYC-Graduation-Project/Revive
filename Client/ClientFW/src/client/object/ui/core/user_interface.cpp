#include "stdafx.h"
#include "client/object/ui/core/user_interface.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	UserInterface::UserInterface(const std::string& name)
		: m_name(name), m_ui_state(eUIState::kActive)
		, m_position(vec2::ZERO) , m_size(Vec2(100.f, 100.f))
	{
	}

	UserInterface::~UserInterface()
	{
	}

	void UserInterface::SetPosition(const Vec2& position)
	{
		m_position = position;
		m_is_updated_transform = true;
	}

	void UserInterface::SetSize(const Vec2& size)
	{
		m_size = size;
		m_is_updated_transform = true;
	}

	void UserInterface::SetTexture(const SPtr<Texture>& texture)
	{
	}
}
