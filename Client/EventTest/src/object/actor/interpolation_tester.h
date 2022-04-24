#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
    class SimpleMovementComponent;
    class CharacterMovementComponent;
}

namespace event_test
{
    using namespace client_fw;

    class InterpolationTester final : public StaticMeshActor
    {
    public:
        InterpolationTester();
        virtual ~InterpolationTester() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;
        virtual void ExecuteMessage(const SPtr<MessageEventInfo>& message) override;
        virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

    private:
        SPtr<SimpleMovementComponent> m_simple_movement_component;
        SPtr<CharacterMovementComponent> m_character_movement_component;

        Vec3 m_velocity;
        Vec3 m_inter_velocity;
        Vec3 m_next_pos;
        Vec3 m_previous_pos;

        float m_time = 0.0f;

    public:
        void SetPreviousPosition(const Vec3& position) { m_previous_pos = m_next_pos = position; }
    };

}


