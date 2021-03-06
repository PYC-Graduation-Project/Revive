#pragma once

const short SERVER_PORT = 9000;

//cm기준, 타일 : 36 x 16
const int  WORLD_HEIGHT = 4800;// 전체맵 크기는 아님
const int  WORLD_WIDTH = 10800;

const int  MAX_NAME_SIZE = 20;
const int  MAX_CHAT_SIZE = 100;

const int  MAX_USER = 3;
const int  MAX_NPC =100;

const float MAX_SPEED=10000; //추후 수정


constexpr int NPC_ID_START = MAX_USER;
constexpr int NPC_ID_END = MAX_USER + MAX_NPC - 1;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_PACKET_ATTACK = 3;
const char CS_PACKET_CHAT = 4;
const char CS_PACKET_TELEPORT = 5;


const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_PACKET_CHAT = 5;
const char SC_PACKET_LOGIN_FAIL = 6;
const char SC_PACKET_STATUS_CHANGE = 7;




#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 0 : 앞,  1: 뒤, 2:왼, 3:오
	int		move_time; //디버그 용 -> 보낸시간 -받은시간 = 통신하는 시간
};

struct cs_packet_attack {
	unsigned char size;
	char	type;
};

struct cs_packet_chat {
	unsigned char size;
	char	type;
	char	message[MAX_CHAT_SIZE];
};

struct cs_packet_teleport { 
	// 서버에서 장애물이 없는 랜덤 좌표로 텔레포트 시킨다.
	// 더미 클라이언트에서 동접 테스트용으로 사용.
	unsigned char size;
	char	type;
};



struct sc_packet_login_ok {
	unsigned char size;
	char type;
	int		id;
	float	x, y, z;
	//short	level;
	//short	hp, maxhp;
	//int		exp;
};


struct sc_packet_move {
	unsigned char size;
	char type;
	int		id;
	float  x, y, z;
	float right_x, right_y, right_z;
	float look_x, look_y, look_z;
};


struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	float x, y, z;
	char object_type;
	char	name[MAX_NAME_SIZE];
};

struct sc_packet_remove_object {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_chat {
	unsigned char size;
	char type;
	int id;
	char message[MAX_CHAT_SIZE];
};

struct sc_packet_login_fail {
	unsigned char size;
	char type;
	int	 reason;		// 0: 중복 ID,  1:사용자 Full
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	int id;
	float	hp, maxhp;
};

#pragma pack(pop)
