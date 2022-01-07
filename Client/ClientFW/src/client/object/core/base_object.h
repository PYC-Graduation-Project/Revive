#pragma once
#include <string_view>

namespace client_fw
{
	class IBaseObject
	{
	public:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Update(float delta_time) = 0;
	};
}
