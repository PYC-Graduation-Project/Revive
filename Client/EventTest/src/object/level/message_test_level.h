#pragma once
#include <client/object/level/core/level.h>

namespace event_test
{
    using namespace client_fw;

    class MessageTestLevel : public Level
    {
    public:
        MessageTestLevel();
        virtual ~MessageTestLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    public:
        virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
        virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const override;
    };

}


