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

    protected:
        SPtr<Pawn> m_controlled_pawn;

    public:
        void Possess(const SPtr<Pawn>& pawn);
        void UnPossess();
    };
}

