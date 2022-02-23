#pragma once
#include "move_object.h"
#include "define.h"
#include<mutex>
#include<atomic>
class Enemy :
    public MoveObj
{
public:
    Enemy() = default;
    ~Enemy() 
    {
        lua_close(L);
    };
    

    //void InitLua(const char* script_name);
    void InitEnemy(OBJ_TYPE type, int room_id, 
        float max_hp, Vector3& pos, float damage,const char* name);
    //void RegisterAPI();
    lua_State* GetLua() { return L; }
    std::atomic_bool in_use = false;
private:
    lua_State* L;
    std::mutex lua_lock;
    
    std::atomic_int target_id;
};

