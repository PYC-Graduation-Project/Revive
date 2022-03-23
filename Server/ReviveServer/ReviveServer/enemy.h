#pragma once
#include "move_object.h"
#include "define.h"
#include<mutex>
#include<atomic>
class Enemy :
    public MoveObj
{
public:
    Enemy(int i) :in_use(false),L(NULL){
        m_id = i;
        target_id = -1;
    };
    ~Enemy() 
    {
        lua_close(L);
    };
    

    //void InitLua(const char* script_name);
    void InitEnemy(OBJ_TYPE type, int room_id, 
        float max_hp, Vector3& pos, float damage,const char* name);
    void SetSpawnPoint(float x,float z);
    Vector3 GetLookVec() { return m_look; }
    //void RegisterAPI();
    lua_State* GetLua() { return L; }
    const int GetTargetId()const { return target_id; }
    void SetTargetId(const int val) { target_id = val; }
    std::atomic_bool in_use;
private:
    lua_State* L;
    std::mutex lua_lock;
    Vector3 m_look;
    std::atomic_int target_id;
};

