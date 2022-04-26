#pragma once
#include "move_object.h"

class Player :
    public MoveObj
{
public:
    Player():m_last_move_time(0),
        m_prev_size(0), m_socket(INVALID_SOCKET)
    {
        //데미지랑 hp초기화 추가해야함
        m_type = OBJ_TYPE::OT_PLAYER;
        m_state = STATE::ST_FREE;
        m_room_id = -1;
        m_damage = PLAYER_DAMAGE;
        ZeroMemory(m_password, MAX_PASSWORD_SIZE + 1);
        m_hp = 100.0f;//추후 밸런스 조정
        m_maxhp = m_hp;
    }
   virtual ~Player()=default;
   std::mutex state_lock;
   int		m_prev_size;
   std::atomic_bool is_matching = false;
private:
    
    EXP_OVER m_recv_over;
    SOCKET  m_socket;
    int		m_last_move_time;
    STATE m_state;
    std::atomic_bool m_is_ready = false;
    char m_password[MAX_PASSWORD_SIZE + 1];
    short m_mach_user_size = 0;
public:

    

    STATE GetState()const { return m_state; }
    void SetState(STATE val) { m_state = val; }
    void DoRecv();
    void DoSend(int num_bytes, void* mess);
    SOCKET& GetSock() { return m_socket; }
    void Init(SOCKET&);
    void ResetPlayer();
    void SetIsReady(bool val) { m_is_ready = val; }
    bool GetIsReady() { return m_is_ready; }
    char* GetPassword() { return m_password; }
    short GetMatchUserSize() { return m_mach_user_size; }
    void SetMatchUserSize(short val) { m_mach_user_size = val; }
};

