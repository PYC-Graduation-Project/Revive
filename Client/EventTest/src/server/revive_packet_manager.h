#pragma once
//#include"server/define.h"
#include<unordered_map>
#include "server/packet_manager.h"
#include"server/network_move_object.h"
class RevivePacketManager :
    public PacketManager
{
public:
    RevivePacketManager()=default;
    virtual~RevivePacketManager() = default;

    virtual void Init()override;
    void ProcessMove(int c_id, unsigned char* p);
    void ProcessSignIn(int c_id, unsigned char* p);
    void ProcessSignUp(int c_id, unsigned char* p);
    void ProcessLoginFali(int c_id, unsigned char* p);
    void ProcessMatching(int c_id, unsigned char* p);
    void ProcessObjInfo(int c_id, unsigned char* p);
    void ProcessTime(int c_id, unsigned char* p);
    void ProcessTest(int c_id, unsigned char* p);
private:
    std::unordered_map<int, client_fw::SPtr<NetworkMoveObj>>m_obj_map;
};

