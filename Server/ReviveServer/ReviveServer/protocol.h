#pragma once

const short SERVER_PORT = 11432;

//cm����, Ÿ�� : 36 x 16
const int  WORLD_HEIGHT = 4800;// ��ü�� ũ��� �ƴ�
const int  WORLD_WIDTH = 10800;

const int  MAX_NAME_SIZE = 20;
const int  MAX_CHAT_SIZE = 100;

const int  MAX_USER = 3;
const int  MAX_NPC =100;

const float MAX_SPEED=10000; //���� ����


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
	char	direction;			// 0 : up,  1: down, 2:left, 3:right
	int		move_time; //����� �� -> �����ð� -�����ð� = ����ϴ� �ð�
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
	// �������� ��ֹ��� ���� ���� ��ǥ�� �ڷ���Ʈ ��Ų��.
	// ���� Ŭ���̾�Ʈ���� ���� �׽�Ʈ������ ���.
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
	int		move_time;
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
	int	 reason;		// 0: �ߺ� ID,  1:����� Full
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	int id;
	float	hp, maxhp;
};

#pragma pack(pop)