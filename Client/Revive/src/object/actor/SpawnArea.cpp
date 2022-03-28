#include <include/client_core.h>
#include "object/actor/SpawnArea.h"

namespace revive
{
	SpawnArea::SpawnArea(const std::vector<SPtr<BoxComponent>>& box_components)
	{
		m_box_components = box_components;
	}
	bool SpawnArea::Initialize()
	{
		bool ret = true;
		
		for (auto& box_component : m_box_components)
			ret &= AttachComponent(box_component);

		return true;
	}
	void SpawnArea::Shutdown()
	{
		for (auto& box_component : m_box_components)
		{
			box_component = nullptr;
		}
	}
	void SpawnArea::Update(float delta_time)
	{
	}
}