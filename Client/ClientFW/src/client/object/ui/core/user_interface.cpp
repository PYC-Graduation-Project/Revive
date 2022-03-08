#include "stdafx.h"
#include "client/object/ui/core/user_interface.h"
#include "client/renderer/core/render.h"
#include "client/input/input.h"

namespace client_fw
{
	UserInterface::UserInterface(const std::string& name, const Vec2& size, size_t num_of_visible_texture)
		: m_name(name), m_position(vec2::ZERO), m_size(size)
	{
		m_visible_textures.resize(num_of_visible_texture);
	}

	UserInterface::~UserInterface()
	{
	}

	bool UserInterface::IsHovered() const
	{
		Vec2 pos = Vec2(static_cast<float>(Input::GetMousePosition().x),
			static_cast<float>(Input::GetMousePosition().y));
		return (pos >= m_position && pos <= m_position + m_size);
	}

	void UserInterface::SetVisibleTexture(const SPtr<UITexture>& texture, UINT index)
	{
		if (index < m_visible_textures.size())
		{
			m_visible_textures[index] = texture;
		}
	}
}
