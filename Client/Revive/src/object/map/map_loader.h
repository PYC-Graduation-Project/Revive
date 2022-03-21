#pragma once


namespace revive
{
	using namespace client_fw;

	enum class eMapLoadType
	{
		kClient,kServer,
	};

	class MapLoader
	{
	public:
		MapLoader() = default;
		~MapLoader() = default;

		std::vector<SPtr<Actor>> LoadMap(const std::string& path, eMapLoadType type);
	};
}