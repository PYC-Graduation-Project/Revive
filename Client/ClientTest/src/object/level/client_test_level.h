#pragma once
#include <client/math/math.h>
#include "client/object/level/core/level.h"

namespace client_test
{
    using namespace client_fw;

    class ClientTestLevel final: public Level
    {
    public:
        ClientTestLevel(std::string_view name = "client_test_level");
        virtual ~ClientTestLevel() = default;

        virtual void Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    private:
        Vec2 m_pos;
    };
}


