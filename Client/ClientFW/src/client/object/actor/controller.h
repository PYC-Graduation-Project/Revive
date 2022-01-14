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

    protected:
        SPtr<Pawn> m_controlled_pawn;
        bool m_updated = false;


    public:
        void Possess(const SPtr<Pawn>& pawn);
        void UnPossess();
        bool IsUpdated() const { return m_updated; }
    };
}

