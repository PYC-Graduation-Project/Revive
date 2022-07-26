#pragma once

const short SERVER_PORT = 9000;

//cm기준, 타일 : 36 x 16
const int  WORLD_HEIGHT = 4800;// 전체맵 크기는 아님
const int  WORLD_WIDTH = 10800;

const int  MAX_NAME_SIZE = 20; // 아이디 최대 사이즈
const int  MAX_PASSWORD_SIZE = 20;// 비밀 번호 최대 사이즈
const int  MAX_CHAT_SIZE = 100;// 채팅 최대 사이즈
const int  MAX_ROOM_SIZE = 2500;//방 최대 사이즈


constexpr int  MAX_USER = MAX_ROOM_SIZE * 3; //최대 동접 가능 인원
const int  NPC_PER_USER = 15;//사람하나당 최대 npc
const int  SORDIER_PER_USER = 9;//사람하나당 최대 해골 병사
const int  KING_PER_USER = 6;//사람하나당 최대 해골킹
constexpr int  MAX_NPC = MAX_USER * NPC_PER_USER; //최대 npc 개수

const float FramePerSecond = 0.05f;
const float SPEED_PER_SECOND = 225.0f;
const float MAX_SPEED = SPEED_PER_SECOND * FramePerSecond; //추후 수정, 플레이어 이동 속도 //225 cm/s
const float MOVE_DISTANCE = 1.0f;//플레이어 이동 거리
const float PLAYER_DAMAGE = 1.0f;
const float FOV_RANGE = 900.0f;

const float SKULL_HP = 5 * PLAYER_DAMAGE;
const float SKULLKING_HP = 10 * PLAYER_DAMAGE;

const float PLAYER_HP = 30.0f;
const float BASE_HP = 50.0f;

const float KING_DAMAGE = 2.0f;
const float SORDIER_DAMAGE = 1.0f;

constexpr int NPC_ID_START = MAX_USER;
constexpr int NPC_ID_END = MAX_USER + MAX_NPC - 1;
constexpr int BASE_ID = NPC_ID_END + 2;



const char CS_PACKET_SIGN_IN = 1;
const char CS_PACKET_SIGN_UP = 2;
const char CS_PACKET_MOVE = 3;
const char CS_PACKET_ATTACK = 4;
const char CS_PACKET_CHAT = 5;
const char CS_PACKET_MATCHING = 6;
const char CS_PACKET_HIT = 7;
const char CS_PACKET_GAME_START = 8;


const char SC_PACKET_SIGN_IN_OK = 1;
const char SC_PACKET_SIGN_UP_OK = 2;
const char SC_PACKET_MOVE = 3;
const char SC_PACKET_PUT_OBJECT = 4;
const char SC_PACKET_REMOVE_OBJECT = 5;
const char SC_PACKET_CHAT = 6;
const char SC_PACKET_LOGIN_FAIL = 7;
const char SC_PACKET_STATUS_CHANGE = 8;
const char SC_PACKET_MATCHING = 9;
const char SC_PACKET_OBJ_INFO = 10;
const char SC_PACKET_TIME = 11;
const char SC_PACKET_TEST = 12;
const char SC_PACKET_NPC_ATTACK = 13;
const char SC_PACKET_ATTACK = 14;
const char SC_PACKET_BASE_STATUS = 15;
const char SC_PACKET_WIN = 16;
const char SC_PACKET_DEFEAT = 17;
const char SC_PACKET_DEAD = 18;
const char SC_PACKET_WAVE_INFO = 19;
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

struct cs_packet_matching {
	unsigned char size;
	char	type;
	short	user_num;//원하는 인원수
};

struct cs_packet_test {
	unsigned char size;
	char	type;
	int id;
	float x, y, z;
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	//char	direction;			// 0 : 앞,  1: 뒤, 2:왼, 3:오
	int		move_time; //디버그 용 -> 보낸시간 -받은시간 = 통신하는 시간
	float x, y, z;


};

struct cs_packet_attack {
	unsigned char size;
	char	type;
	float f_x, f_y, f_z;
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

struct cs_packet_hit {
	// 서버에서 장애물이 없는 랜덤 좌표로 텔레포트 시킨다.
	// 더미 클라이언트에서 동접 테스트용으로 사용.
	unsigned char size;
	char	type;
	int victim_id;
	int attacker_id;
};

struct cs_packet_game_start {
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
struct sc_packet_matching {//예전 login_ok처럼 player초기화 보내주기
	unsigned char size;
	char	type;
	//short	user_num;//원하는 인원수
};

struct sc_packet_sign_up_ok {
	unsigned char size;
	char type;
};
struct sc_packet_wave_info {
	unsigned char size;
	char type;
	int curr_round;
	int sordier_num;
	int king_num;
};
struct sc_packet_npc_attack {
	unsigned char size;
	char type;
	int obj_id;
	int target_id;
};
struct sc_packet_move {
	unsigned char size;
	char type;
	int		id;
	float x, y, z;
	int move_time;
};
struct sc_packet_test {
	unsigned char size;
	char	type;
	int id;
	float speed;
	float x, y, z;
};
struct sc_packet_obj_info {
	unsigned char size;
	char type;
	int id;
	float maxhp;
	float damage;
	float x, y, z;
	char object_type;
	char color_type;
	char name[MAX_NAME_SIZE + 2];
};

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	float x, y, z;
	char object_type;
	char name[MAX_NAME_SIZE];
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
struct sc_packet_time {//예전 login_ok처럼 player초기화 보내주기
	unsigned char size;
	char	type;
	float time;
	int send_time;//추후에 없애야될수도 있음

};
struct sc_packet_attack {
	unsigned char size;
	char type;
	int	obj_id;
	float f_x, f_y, f_z;

};
struct sc_packet_base_status {
	unsigned char size;
	char type;
	int	room_id;
	float hp;

};
struct sc_packet_dead {
	unsigned char size;
	char type;
	int	obj_id;

};
struct sc_packet_win {
	unsigned char size;
	char type;


};
struct sc_packet_defeat {
	unsigned char size;
	char type;


};
#pragma pack(pop)
