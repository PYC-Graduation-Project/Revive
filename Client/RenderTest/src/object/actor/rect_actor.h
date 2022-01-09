#pragma once
#include <client/object/actor/core/actor.h>

namespace render_test
{
    using namespace client_fw;

    class RectActor : public Actor
    {
	public:
		RectActor();
		virtual ~RectActor() = default;

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
    };


}

