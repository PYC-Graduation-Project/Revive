#pragma once
#include "iocp_server.h"
class InGameServer :
    public IOCPServer
{
public:
    InGameServer();
    ~InGameServer();

    void Worker()override;

};

