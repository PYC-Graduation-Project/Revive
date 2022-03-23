#pragma once
#include "server/network_object.h"
#include"client/math/quaternion.h"
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
    virtual~NetworkMoveObj();

    const client_fw::Quaternion& GetRotation() const { return m_rotation; }
    void SetRotation(const client_fw::Quaternion& rot) { m_rotation = rot; }

    const float GetDamage()const { return m_damage; }
    void SetDamage(float val) { m_damage = val; }
private:
    float m_damage;
    client_fw::Quaternion m_rotation;
};

