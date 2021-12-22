#pragma once

#include <memory>

namespace client_fw
{
	template<class T>
	using UPtr = std::unique_ptr<T>;
	template<class T, class ... Args>
	constexpr UPtr<T> CreateUPtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SPtr<T> CreateSPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WPtr = std::weak_ptr<T>;
}