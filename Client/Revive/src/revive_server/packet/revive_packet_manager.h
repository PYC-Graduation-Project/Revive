#pragma once
//#include"server/define.h"
#include<unordered_map>
#include "server/packet_manager.h"
#include"server/network_move_object.h"
#include"revive_server/game_info.h"
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
    void ProcessNpcAttack(int c_id, unsigned char* p);
    void ProcessAttack(int c_id, unsigned char* p);
    void ProcessBaseStatus(int c_id, unsigned char* p);
    void ProcessStatusChange(int c_id, unsigned char* p);
private:
    std::unordered_map<int, client_fw::SPtr<NetworkMoveObj>>m_obj_map;
    GameInfo m_game_info;
};

