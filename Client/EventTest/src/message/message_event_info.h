#pragma once
#include <client/event/messageevent/message_event_info.h>

namespace event_test
{
    using namespace client_fw;

    class RotSpeedMessageEventInfo final : public MessageEventInfo
    {
    public:
        RotSpeedMessageEventInfo(UINT event_id, float speed);

    private:
        float m_speed;

    public:
        float GetSpeed() const { return m_speed; }
    };
}



