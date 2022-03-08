#pragma once

#include <WS2tcpip.h>
#include <MSWSock.h>
#include<chrono>
#include"state.h"
#include"vec3.h"
#include"vec4.h"
#include"protocol.h"




extern "C" {
#include "include\lua.h"
#include "include\lauxlib.h"
#include "include\lualib.h"
}

#pragma comment (lib, "lua54.lib")

const int BUFSIZE = 256;



class EXP_OVER
{
public:
	WSAOVERLAPPED _wsa_over;
	COMP_OP		  _comp_op;
	WSABUF		  _wsa_buf;
	unsigned char _net_buf[BUFSIZE];
	int target_id;
	int room_id;
public:
	EXP_OVER(COMP_OP comp_op, char num_bytes, void* mess) : _comp_op(comp_op)
	{
		ZeroMemory(&_wsa_over, sizeof(_wsa_over));
		_wsa_buf.buf = reinterpret_cast<char*>(_net_buf);
		_wsa_buf.len = num_bytes;
		memcpy(_net_buf, mess, num_bytes);
	};
	EXP_OVER(COMP_OP comp_op) :_comp_op(comp_op) {}
	EXP_OVER() { _comp_op = COMP_OP::OP_RECV; }
	~EXP_OVER() {}


};

struct timer_event {
	int obj_id;
	int room_id;
	std::chrono::system_clock::time_point	start_time;
	EVENT_TYPE ev;
	int target_id;
	constexpr bool operator < (const timer_event& _Left) const
	{
		return (start_time > _Left.start_time);
	}
};

struct db_task {
	int obj_id;
	DB_TASK_TYPE dt;
	char user_id[MAX_NAME_SIZE];
	char user_password[MAX_PASSWORD_SIZE];
};

