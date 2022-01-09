#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include "object/level/render_rect_level.h"
#include "object/actor/rect_actor.h"

#include <random>

namespace render_test
{
	RenderRectLevel::RenderRectLevel(const std::string& name)
		: Level(name)
	{
	}

	void RenderRectLevel::Initialize()
	{
		

		RegisterPressedEvent("spawn rect actor", { EventKeyInfo{eKey::kS} },
			[this]()->bool {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> uid_position(-0.5f, 0.5f);
				auto actor = CreateSPtr<RectActor>();
				SpawnActor(actor);
				actor->SetPosition(Vec3(uid_position(gen), uid_position(gen), 0.01f));
				m_actors.push(actor);
				return true;
			});

		RegisterPressedEvent("kill rect actor", { EventKeyInfo{eKey::kD} },
			[this]()->bool {
				if (m_actors.empty() == false)
				{
					m_actors.front()->SetActorState(eActorState::kDead);
					m_actors.pop();
				}
				return true;
			});
	}

	void RenderRectLevel::Shutdown()
	{
	}

	void RenderRectLevel::Update(float delta_time)
	{
	}
}
