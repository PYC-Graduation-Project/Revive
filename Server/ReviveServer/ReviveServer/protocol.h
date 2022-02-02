#pragma once

const short SERVER_PORT = 9000;

//cm����, Ÿ�� : 36 x 16
const int  WORLD_HEIGHT = 4800;// ��ü�� ũ��� �ƴ�
const int  WORLD_WIDTH = 10800;

const int  MAX_NAME_SIZE = 20; // ���̵� �ִ� ������
const int  MAX_PASSWORD_SIZE = 20;// ��� ��ȣ �ִ� ������
const int  MAX_CHAT_SIZE = 100;// ä�� �ִ� ������
const int MAX_ROOM_SIZE = 25;//�� �ִ� ������


constexpr int  MAX_USER = MAX_ROOM_SIZE*3; //�ִ� ���� ���� �ο�
const int  NPC_PER_USER = 15;//����ϳ��� �ִ� npc
constexpr int  MAX_NPC = MAX_USER* NPC_PER_USER; //�ִ� npc ����

const float MAX_SPEED=10000; //���� ����, �÷��̾� �̵� �ӵ�
const float MOVE_DISTANCE = 1.0f;//�÷��̾� �̵� �Ÿ�

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
	char	direction;			// 0 : ��,  1: ��, 2:��, 3:��
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
	char reason;		// 1: DBerror,  2:����� Full, 3:�̹� ������, 4:���Ʋ��, 5:���̵����, 6:�ش���̵� �̹�����
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	int id;
	float	hp, maxhp;
};

#pragma pack(pop)
