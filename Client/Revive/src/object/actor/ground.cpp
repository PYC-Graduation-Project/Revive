#include <include/client_core.h>
#include "object/actor/ground.h"

namespace revive
{
	Ground::Ground(const std::vector<SPtr<StaticMeshComponent>>& blocks)
		: Actor(eMobilityState::kStatic)
	{
		m_blocks = blocks;
	}
	
	bool Ground::Initialize()
	{
		bool ret = true;
		for ( auto& block : m_blocks)
			AttachComponent(block);
		return ret;
	}
	void Ground::Shutdown()
	{
	}
}