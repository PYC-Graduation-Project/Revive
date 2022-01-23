
#include "pch.h"

#include"ingame_server.h"
using namespace std;
const int MAX_THREAD = 12;


int main()
{
    InGameServer server;
    server.Init(1);
    cout << "초기화완료" << endl;
    server.BindListen(SERVER_PORT);
    cout << "바인드 리슨 중" << endl;
   
 
    server.Run();
    cout << "동작 끝" << endl;
    server.End();
  
    return 0;
}

