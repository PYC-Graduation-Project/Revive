#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	class ImageUI : public UserInterface
	{
	public:
		ImageUI(const std::string& name = "Image");

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture);

	protected:
		SPtr<UITexture> m_normal_texture;
	};

}

