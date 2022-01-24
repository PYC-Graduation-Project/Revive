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
		virtual SPtr<MeshOctree> CreateMeshOctree() const override;
	};
}



