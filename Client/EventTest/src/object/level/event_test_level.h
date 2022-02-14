#pragma once
#include <client/object/level/core/level.h>

namespace event_test
{
	using namespace client_fw;

	class EventTestLevel : public Level
	{
	public:
		EventTestLevel();
		virtual ~EventTestLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time);

	public:
		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
		virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const override;

	private:
		Vec3 m_spawn_pos;
	};
}



