#pragma once
#include "server/network_object.h"
#include"client/math/quaternion.h"
enum class NW_OBJ_TYPE
{
    OT_NONE,
    OT_MY_PLAYER,
    OT_BASE=3,
    OT_PLAYER,
    OT_NPC_SKULL,
    OT_NPC_SKULLKING
};
class NetworkMoveObj :
    public NetworkObj
{
public:
    NetworkMoveObj();
    NetworkMoveObj(int id, float hp, char* name,
        float x, float y, float z, NW_OBJ_TYPE type,float damage)
    {
        m_id = id;
        m_name = std::string(name);
        m_hp = hp;
        m_max_hp = hp;
        m_position = client_fw::Vec3(x, y, z);
        m_type = type;
        m_damage = damage;
        m_rotation = client_fw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    }
    NetworkMoveObj(const NetworkMoveObj& other)
    {
        m_id = other.GetID();
        m_name = std::string(other.GetName());
        m_hp = other.GetHp();
        m_max_hp = other.GetMaxHp();
        m_position = other.GetPosition();
        m_type = other.GetType();
        m_damage = other.GetDamage();
        m_rotation = client_fw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    }
    ~NetworkMoveObj();

    void SetType(const NW_OBJ_TYPE val) { m_type = val; }
    void SetHP(float val) { m_hp = val; }
    void SetMaxHP(float val) { m_max_hp = val; }
    const float GetHp()const { return m_hp; }
    const float GetMaxHp()const { return m_max_hp; }
    const NW_OBJ_TYPE GetType()const { return m_type; }
    const float GetDamage()const { return m_damage; }
    void SetDamage(float val) { m_damage = val; }
private:
    float m_hp, m_max_hp;
    NW_OBJ_TYPE m_type;
    float m_damage;
    //client_fw::Quaternion m_rotation;
};

