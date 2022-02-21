#include "stdafx.h"
#include "client/object/ui/image_ui.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	ImageUI::ImageUI(const std::string& name)
		: UserInterface(name)
	{
	}

	void ImageUI::SetTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ImageUI::SetTexture(const SPtr<Texture>& texture)
	{
		m_texture = texture;
	}
}
