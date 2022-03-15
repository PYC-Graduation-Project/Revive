#pragma once
#include "server/send_manager.h"

class ReviveSendManager :
    public SendManager
{
public:
    ReviveSendManager()=default;
    virtual ~ReviveSendManager()=default;
    virtual void ProcessSend(const SOCKET&s_socket,const client_fw::SPtr<client_fw::MessageEventInfo>& message)override;

    void SendSignInPacket  (const SOCKET&s_socket,char*, char*);
    void SendSignUPPacket  (const SOCKET&s_socket,char*, char*);
    void SendMatchingPacket(const SOCKET&s_socket,int user_num);
    void SendMovePacket    (const SOCKET&s_socket,client_fw::Vec3& pos, client_fw::Quaternion& rot);
};

