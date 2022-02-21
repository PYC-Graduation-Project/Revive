#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	class ImageUI : public UserInterface
	{
	public:
		ImageUI(const std::string& name = "Image");

	public:
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture);
	};

}

