#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
    class Pawn;

    class Controller : public Actor
    {
    protected:
        Controller(const std::string& name = "controller");
        virtual ~Controller() = default;

        virtual void Update(float delta_time) override;
        
    private:
        void UpdatePitch(float delta_time);
        void UpdateRoll(float delta_time);

    protected:
        SPtr<Pawn> m_controlled_pawn;
        float m_pitch_speed = 0.0f;
        float m_yaw_speed = 0.0f;
        float m_roll_speed = 0.0f;

    public:
        void Possess(const SPtr<Pawn>& pawn);
        void UnPossess();
    };
}

