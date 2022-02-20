#pragma once


enum class EVENT_TYPE { 
	EVENT_NPC_SPAWN,
	EVENT_NPC_MOVE, 
	EVENT_PLAYER_MOVE, 
	EVENT_NPC_ATTACK, 
	EVENT_REGEN 
};

enum class COMP_OP {
	OP_RECV, 
	OP_SEND, 
	OP_ACCEPT, 
	OP_NPC_SPAWN,
	OP_NPC_MOVE, 
	OP_PLAYER_MOVE, 
	OP_PLAYER_HILL,
	OP_TRACKING_PLAYER, 
	OP_NPC_ATTACK,
	OP_REGEN
};

enum class STATE 
{ 
	ST_FREE, 
	ST_ACCEPT, 
	ST_INGAME 
};

enum class OBJ_TYPE
{
	OT_MAPOBJ,
	OT_PLAYER,
	OT_NPC_SKULL,
	OT_NPC_SKULLKING
};

enum class LOGINFAIL_TYPE {
	OK,
	DB_ERROR,
	FULL,// 서버에 사용자가 너무 많을 때
	AREADY_SIGHN_IN,//이미 해당아이디로 접속한 플레이어가 있을 때
	WRONG_PASSWORD,//비밀번호가 틀렸을 때
	NO_ID// 회원가입 안했을 때(db에 정보 없음)
};

enum class DB_TASK_TYPE {
	SIGN_IN,
	SIGN_UP
};