#pragma once
#include "iocp/iocp_server.h"

class PacketManager;
class InGameServer :
    public IOCPServer
{
public:
    InGameServer();
    virtual ~InGameServer();
    
   
    virtual bool OnAccept( EXP_OVER* exp_over) override;
    virtual bool OnRecv(int c_id, EXP_OVER* exp_over,DWORD num_bytes) override;
    virtual void OnEvent(EXP_OVER* exp_over)override;
    virtual void Disconnect(int c_id)override;
    
    void CreateTimer();
    void DoTimer(HANDLE hiocp);
    void Run();
    void End();
private:
    std::unique_ptr< PacketManager>m_PacketManager;
   
    
};

