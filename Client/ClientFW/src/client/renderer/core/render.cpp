#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
//#include "client/renderer/renderlevel/core/render_level.h"
//#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	std::string Render::ConvertRenderLevelType(eRenderLevelType type)
	{
		switch (type)
		{
		case eRenderLevelType::kDefault:
			return "default";
		default:
			return "unknown";
		}
	}
}
