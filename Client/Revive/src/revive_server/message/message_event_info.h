#pragma once
#include <client/event/messageevent/message_event_info.h>
#include"server/protocol.h"
#include<mutex>
enum class NW_OBJ_TYPE;
class NetworkMoveObj;
namespace revive
{
    using namespace client_fw;
   
    class NpcAttackEventInfo final : public MessageEventInfo
    {
    public:

        NpcAttackEventInfo(UINT event_id, const Vec3& target_pos);

    private:
        
        Vec3 m_target_pos;


    public:
       
        const Vec3& GetTargetPosition() const { return m_target_pos; }
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

    class SignInMessageOkEventInfo final :public MessageEventInfo
    {
    public:
        SignInMessageOkEventInfo(UINT event_id);
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
        TestMessageEventInfo(UINT event_id,float speed,float x,float y,float z)
            :MessageEventInfo(event_id),m_speed(speed)
        {
            m_vec = Vec3(x, y, z);
        };
    private:
        
        Vec3 m_vec;
        float m_speed;
    public:
        const Vec3& GetPosition() const{ return m_vec; }
        const float GetObjId()const { return m_speed; }
    };

    class ObjectInfoMessageEventInfo final :public MessageEventInfo
    {
    public:
        ObjectInfoMessageEventInfo(UINT event_id,const SPtr<NetworkMoveObj>& other);
        std::mutex m_obj_lock;
    private:
        SPtr<NetworkMoveObj>m_network_object;
    public:
        const SPtr<NetworkMoveObj>& GetNetworkObj()const { return m_network_object; }
    };
   
    class MoveObjectMessageEventInfo final :public MessageEventInfo
    {
    public:
        MoveObjectMessageEventInfo(UINT event_id, const Vec3& pos);
       
    private:
        Vec3 m_pos;
        
       
    public:
        const Vec3& GetObjPosition()const { return m_pos; }
      
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

    class SendAttackEventInfo final : public MessageEventInfo
    {
    public:
        SendAttackEventInfo(UINT event_id,const Vec3&start_pos, const Vec3&forward_vec);
    private:
        Vec3 m_start_pos;
        Vec3 m_forward_vec;
    public:
        const Vec3& GetStartPosition()const { return m_start_pos; }
        const Vec3& GetForward()const { return m_forward_vec; }
    };
    class RecvAttackEventInfo final : public MessageEventInfo
    {
    public:
        RecvAttackEventInfo(UINT event_id);
  
    };
    class MatchingMessageOKEventInfo final :public MessageEventInfo
    {
    public:
        MatchingMessageOKEventInfo(UINT event_id);
   
    };
    class ObjectHitMessageEventInfo final :public MessageEventInfo
    {
    public:
        ObjectHitMessageEventInfo(UINT event_id,int victim_id, int attacker_id);
    private:
        int m_attacker_id;
        int m_victim_id;
    public:
        const int GetVictimID() { return m_victim_id; }
        const int GetAttackerID() { return m_attacker_id; }
    };
    class BaseHpChangeEventInfo final :public MessageEventInfo
    {
    public:
        BaseHpChangeEventInfo(UINT event_id, float base_hp);
    private:
        float m_base_hp;
    public:
        const float GetBaseHp() { return m_base_hp; }
    };
    class StatusChangeEventInfo final :public MessageEventInfo
    {
    public:
        StatusChangeEventInfo(UINT event_id, float obj_hp);
    private:
        float m_obj_hp;
      
    public:
       
        const float GetObjHp() { return m_obj_hp; }
    };

    class GameStartEventInfo final : public MessageEventInfo
    {
    public:
        GameStartEventInfo(UINT event_id);
    };

    class ObjectDeadEventInfo final : public MessageEventInfo
    {
    public:
        ObjectDeadEventInfo(UINT event_id);
    };

    class GameWinEventInfo final : public MessageEventInfo
    {
    public:
        GameWinEventInfo(UINT event_id);
    };

    class GameDefeatEventInfo final : public MessageEventInfo
    {
    public:
        GameDefeatEventInfo(UINT event_id);
    };
}
