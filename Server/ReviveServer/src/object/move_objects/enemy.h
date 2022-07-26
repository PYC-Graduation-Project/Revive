#pragma once
#include "move_object.h"
#include "define.h"
#include"util/collision/collisioner.h"
#include<mutex>
#include<atomic>
#include<vector>
const int SYNC_TIME = 30;

class Enemy :
    public MoveObj
{
public:
    Enemy(int i) :in_use(false),m_L(NULL){
        m_id = i;
        target_id = BASE_ID;
        m_color_type = COLOR_TYPE::CT_NONE;
        m_attack_time = std::chrono::system_clock::now();
        m_check_time= std::chrono::system_clock::now();
        m_move_time= std::chrono::system_clock::now();
    };
    ~Enemy() 
    {
        lua_close(m_L);
    };
    

  
    void InitEnemy(OBJ_TYPE type, int room_id, 
        float max_hp, Vector3& pos, float damage,const char* name);
    void SetSpawnPoint(float x,float z);
    Vector3& GetLookVec()  { return m_look; }
    //Vector3& GetPrevLookVec() { return m_prev_look; }
    //void SetPrevLookVec(const Vector3& val) { m_prev_look=val; }
    Vector3& GetPrevPos() { return m_prev_pos; }
    Vector3& GetPrevTestPos() { return m_prev_test_pos; }
    BoxCollision& GetCollision() { return m_collision; }
    BoxCollision& GetPrevCollision() { return m_prev_collision; }
    void SetCollision(const BoxCollision& val) { m_collision = val; }
    void SetPrevCollision(const BoxCollision& val) { m_prev_collision = val; }
    void SetPrevTestPos(const Vector3& val) { m_prev_test_pos = val; }
    virtual void Reset()override;

    lua_State* GetLua() { return m_L; }
    const int GetTargetId()const { return target_id; }
    void SetTargetId(const int val) { target_id = val; }
    void SetMoveTime(int val); 
    std::chrono::system_clock::time_point& GetAttackTime() { return m_attack_time; }
    std::chrono::system_clock::time_point& GetCheckTime() { return m_check_time; }
    void DoMove(const Vector3& target_pos);
    void DoPrevMove(const Vector3& target_pos);
    void SetToPrevPos() { 
        m_pos = m_prev_pos; 
        m_collision.UpdateCollision(m_pos);
    }
    
    std::atomic_bool in_use;
    std::atomic_bool in_game=false;
    std::mutex lua_lock;
    Vector3 m_prev_test_pos{ 0.0f,0.0f,0.0f };
    std::chrono::system_clock::time_point	m_move_time;
private:
    //std::vector<Vector3>m_load;
    std::chrono::system_clock::time_point	m_attack_time;
    std::chrono::system_clock::time_point	m_check_time;
    BoxCollision m_collision;
    BoxCollision m_prev_collision;
    Vector3 m_prev_pos{ 0.0f,0.0f,0.0f };
    //Vector3 m_prev_look;
    lua_State* m_L;
    Vector3 m_look;
    std::atomic_int target_id;
};

