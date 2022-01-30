#pragma once
#include "move_object.h"

class Player :
    public MoveObj
{
public:
    Player()
    {
        
        m_last_move_time = 0;
        m_prev_size = 0;
        m_socket= INVALID_SOCKET;
    }
   virtual ~Player()=default;
   std::mutex state_lock;
   int		m_prev_size;
private:
    
    EXP_OVER m_recv_over;
    SOCKET  m_socket;
    int		m_last_move_time;
    char m_name[MAX_NAME_SIZE + 1];
    char m_password[MAX_PASSWORD_SIZE + 1];
public:
    void DoRecv();
    void DoSend(int num_bytes, void* mess);
    SOCKET& GetSock() { return m_socket; }
    void Init(SOCKET&);
    char* GetName() { return m_name; }
    char* GetPassword() { return m_password; }
};

