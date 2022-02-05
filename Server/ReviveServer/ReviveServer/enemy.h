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
    ~Enemy() = default;
    
private:
    lua_State* L;
    std::mutex lua_lock;
    std::atomic_int target_id;
};

