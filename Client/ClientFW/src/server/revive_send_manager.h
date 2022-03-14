#pragma once
#include "server/send_manager.h"

class ReviveSendManager :
    public SendManager
{
public:
    ReviveSendManager()=default;
    virtual ~ReviveSendManager()=default;
    virtual void SendByMessageToSrver(const client_fw::SPtr<client_fw::MessageEventInfo>& message)override;

    void SendSignInPacket(char*, char*);
    void SendSignUPPacket(char*, char*);
    void SendMatchingPacket(int user_num);
    void SendMovePacket(client_fw::Vec3& pos, client_fw::Quaternion& rot);
};

