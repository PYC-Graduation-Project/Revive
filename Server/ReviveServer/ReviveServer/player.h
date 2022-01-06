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
private:
    
    EXP_OVER m_recv_over;
    SOCKET  m_socket;
    int		m_prev_size;
    int		m_last_move_time;

public:
    void DoRecv();
    void DoSend(int num_bytes, void* mess);
    SOCKET& GetSock() { return m_socket; }
    void Init(SOCKET&);
};

