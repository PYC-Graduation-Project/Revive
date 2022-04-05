#pragma once
#include "move_object.h"
#include "define.h"
#include<mutex>
#include<atomic>
const int SYNC_TIME = 30;

class Enemy :
    public MoveObj
{
public:
    Enemy(int i) :in_use(false),m_L(NULL){
        m_id = i;
        target_id = -1;
        
    };
    ~Enemy() 
    {
        lua_close(m_L);
    };
    

    //void InitLua(const char* script_name);
    void InitEnemy(OBJ_TYPE type, int room_id, 
        float max_hp, Vector3& pos, float damage,const char* name);
    void SetSpawnPoint(float x,float z);
    Vector3& GetLookVec()  { return m_look; }
    //void RegisterAPI();
    lua_State* GetLua() { return m_L; }
    const int GetTargetId()const { return target_id; }
    void SetTargetId(const int val) { target_id = val; }
    void SetMoveTime(int val); 
    const std::chrono::system_clock::time_point& GetMoveTime() { return m_move_time; }
    std::atomic_bool in_use;
    std::mutex lua_lock;
private:
    std::chrono::system_clock::time_point	m_move_time;
    lua_State* m_L;
    Vector3 m_look;
    std::atomic_int target_id;
};

