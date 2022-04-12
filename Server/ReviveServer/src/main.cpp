
#include "pch.h"

#include"game_server/ingame_server.h"
using namespace std;
const int MAX_THREAD = 12;


int main()
{
    setlocale(LC_ALL, "korean");
    wcout.imbue(locale("korean"));
    InGameServer server;
    server.Init(MAX_THREAD);
    cout << "초기화완료" << endl;
    server.BindListen(SERVER_PORT);
    cout << "바인드 리슨 중" << endl;
   
 
    server.Run();
    cout << "동작 끝" << endl;
    server.End();
  
    return 0;
}

