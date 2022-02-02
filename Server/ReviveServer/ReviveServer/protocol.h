#pragma once

const short SERVER_PORT = 9000;

//cm기준, 타일 : 36 x 16
const int  WORLD_HEIGHT = 4800;// 전체맵 크기는 아님
const int  WORLD_WIDTH = 10800;

const int  MAX_NAME_SIZE = 20; // 아이디 최대 사이즈
const int  MAX_PASSWORD_SIZE = 20;// 비밀 번호 최대 사이즈
const int  MAX_CHAT_SIZE = 100;// 채팅 최대 사이즈
const int MAX_ROOM_SIZE = 25;//방 최대 사이즈


constexpr int  MAX_USER = MAX_ROOM_SIZE*3; //최대 동접 가능 인원
const int  NPC_PER_USER = 15;//사람하나당 최대 npc
constexpr int  MAX_NPC = MAX_USER* NPC_PER_USER; //최대 npc 개수

const float MAX_SPEED=10000; //추후 수정, 플레이어 이동 속도
const float MOVE_DISTANCE = 1.0f;//플레이어 이동 거리

constexpr int NPC_ID_START = MAX_USER;
constexpr int NPC_ID_END = MAX_USER + MAX_NPC - 1;

const char CS_PACKET_SIGN_IN = 1;
const char CS_PACKET_SIGN_UP = 2;
const char CS_PACKET_MOVE = 3;
const char CS_PACKET_ATTACK = 4;
const char CS_PACKET_CHAT = 5;
const char CS_PACKET_TELEPORT = 6;


const char SC_PACKET_SIGN_IN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_PACKET_CHAT = 5;
const char SC_PACKET_LOGIN_FAIL = 6;
const char SC_PACKET_STATUS_CHANGE = 7;
const char SC_PACKET_SIGN_UP_OK = 8;



#pragma pack (push, 1)
struct cs_packet_sign_in {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
	char	password[MAX_PASSWORD_SIZE];
};

struct cs_packet_sign_up {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
	char	password[MAX_PASSWORD_SIZE];
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



struct sc_packet_sign_in_ok {
	unsigned char size;
	char type;
	int		id;
	float	x, y, z;
	//short	level;
	//short	hp, maxhp;
	//int		exp;
};

struct sc_packet_sign_up_ok {
	unsigned char size;
	char type;
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
	char reason;		// 1: DBerror,  2:사용자 Full, 3:이미 접속중, 4:비번틀림, 5:아이디없음, 6:해당아이디 이미존재
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	int id;
	float	hp, maxhp;
};

#pragma pack(pop)
