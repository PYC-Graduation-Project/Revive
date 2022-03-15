#pragma once
#include <client/event/messageevent/message_event_info.h>
#include"server/protocol.h"
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

    class SignInMessageEventInfo final :public MessageEventInfo
    {
    public:
        SignInMessageEventInfo(UINT event_id, char* id, char* pw);
    private:
        char m_user_id[MAX_NAME_SIZE + 1];
        char m_user_pw[MAX_PASSWORD_SIZE + 1];
    public:
        char* GetUserID() { return m_user_id; }
        char* GetUserPassword() { return m_user_pw; }
    };
}



