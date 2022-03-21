#include <include/client_core.h>
#include "object/actor/ground.h"

namespace revive
{
	Ground::Ground(const std::string& path)
		:Actor(eMobilityState::kStatic)
	{
		m_blocks.resize(5);
		float i = 30;
		for (auto& block : m_blocks)
		{
			block = CreateSPtr<StaticMeshComponent>();
			block->SetMesh(path);
			block->SetLocalPosition(Vec3{0.0f, 0.0f, 0.0f+i});
			block->SetLocalScale(Vec3{30.0f, 30.0f, 30.0f});
			i += 30.0f;
		}
	}
	Ground::Ground(const std::vector<SPtr<StaticMeshComponent>>& blocks)
		: Actor(eMobilityState::kStatic)
	{
		m_blocks = blocks;
	}

	bool Ground::Initialize()
	{
		bool ret = true;
		for (auto& block : m_blocks)
			ret &= AttachComponent(block);
		return ret;
	}
	void Ground::Shutdown()
	{
		for (auto& block : m_blocks)
		{
			block = nullptr;
		}
	}
}