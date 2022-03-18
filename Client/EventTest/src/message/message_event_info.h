#pragma once
#include <client/event/messageevent/message_event_info.h>
#include"server/protocol.h"
enum class NW_OBJ_TYPE;
class NetworkMoveObj;
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

    class SignUpMessageEventInfo final :public MessageEventInfo
    {
    public:
        SignUpMessageEventInfo(UINT event_id, char* id, char* pw);
    private:
        char m_user_id[MAX_NAME_SIZE + 1];
        char m_user_pw[MAX_PASSWORD_SIZE + 1];
    public:
        char* GetUserID() { return m_user_id; }
        char* GetUserPassword() { return m_user_pw; }
    };
    class TestMessageEventInfo final :public MessageEventInfo
    {
    public:
        TestMessageEventInfo(UINT event_id,int obj_id,float x,float y,float z)
            :MessageEventInfo(event_id),m_obj_id(obj_id)
        {
            m_position = Vec3(x, y, z);
        };
    private:
        int m_obj_id;
        Vec3 m_position;
    public:
        const Vec3& GetPosition() const{ return m_position; }
        const int GetObjId()const { return m_obj_id; }
    };

    class ObjectInfoMessageEventInfo final :public MessageEventInfo
    {
    public:
        ObjectInfoMessageEventInfo(UINT event_id,const SPtr<NetworkMoveObj>& other);
    private:
        SPtr<NetworkMoveObj>m_network_object;
    public:
        const SPtr<NetworkMoveObj>& GetNetworkObj()const { return m_network_object; }
    };

    class MatchingMessageEventInfo final :public MessageEventInfo
    {
    public:
        MatchingMessageEventInfo(UINT event_id, int user_num);
    private:
        int m_user_num;
        public:
        int GetUserNum()const { return m_user_num; }
    };
}



