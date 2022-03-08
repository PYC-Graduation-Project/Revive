#include"stdafx.h"
#include "packet_manager.h"
#include"network_move_object.h"
#include"network_obj_manager.h"
#include<iostream>
using namespace std;

PacketManager::PacketManager()
{

}

PacketManager::~PacketManager()
{
}

void PacketManager::Init()
{

}

void PacketManager::ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte)
{
	int remain_data = num_byte + m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(client_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type)
	{
	case SC_PACKET_MOVE: {
		ProcessMove(c_id, p);
		break;
	}
	case SC_PACKET_SIGN_IN_OK: {
		ProcessSignIn(c_id, p);
		break;
	}
	case SC_PACKET_SIGN_UP_OK: {
	
		ProcessSignUp(c_id, p);

		break;
	}
	case SC_PACKET_LOGIN_FAIL: {
		ProcessLoginFali(c_id, p);
		break;
	}
	case SC_PACKET_MATCHING: {
		ProcessMatching(c_id, p);
		break;
	}
	case SC_PACKET_OBJ_INFO: {
		ProcessObjInfo(c_id, p);
		break;
	}
	}
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	//cout << "Packetx :" << move_packet->x << ", y : " << move_packet->y << ", z : " << move_packet->z << endl;
	NetworkMoveObj*mover=static_cast<NetworkMoveObj*>(NetworkObjManager::GetInst()->GetObj(packet->id));
	if (!mover) { 
		cout << "없는 객체입니다!" << endl; 
		return;
	}
	client_fw::Vec3 pos{ packet->x,packet->y ,packet->z };
	client_fw::Quaternion rot{ packet->r_x,packet->r_y ,packet->r_z ,packet->r_w };
	mover->SetPosition(pos);
	mover->SetRotation(rot);

}

void PacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_id = packet->id;
	cout << "로그인 성공" << endl;
	

}

void PacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "회원가입 성공" << endl;
}

void PacketManager::ProcessLoginFali(int c_id, unsigned char* p)
{
	sc_packet_login_fail* packet = reinterpret_cast<sc_packet_login_fail*>(p);
	//cout << "로그인 실패" << endl;
	switch (packet->reason)
	{
	case 1: {
		cout << "DBError" << endl;
		break;
	}
	case 2: {
		cout << "사용자 Full" << endl;
		break;
	}
	case 3: {
		cout << "이미 접속중" << endl;
		break;
	}
	case 4: {
		cout << "비번틀림" << endl;
		break;
	}
	case 5: {
		cout << "아이디없음" << endl;
		break;
	}
	case 6: {
		cout << "해당 아이디 존재" << endl;
		break;
	}
	}
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "매칭성사" << endl;
}

void PacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	if (packet->object_type == static_cast<char>(NW_OBJ_TYPE::OT_BASE))
	{
		obj = new NetworkObj(
			packet->id,
			packet->maxhp,
			packet->name,
			packet->x,
			packet->y,
			packet->z,
			(NW_OBJ_TYPE)packet->object_type
		);
		//NetworkObjManager::GetInst()->AddObj(packet->id, obj);
	}
	else
	{
		obj = new NetworkMoveObj(
			packet->id,
			packet->maxhp,
			packet->name,
			packet->x,
			packet->y,
			packet->z,
			(NW_OBJ_TYPE)packet->object_type,
			packet->damage
		);

	}
	NetworkObjManager::GetInst()->AddObj(packet->id, obj);
}

void PacketManager::SendPacket(int num_byte, void* packet)
{
	EXP_OVER* exp_over = new EXP_OVER(COMP_OP::OP_SEND, num_byte, packet);;
	int ret = WSASend(m_s_socket, &exp_over->_wsa_buf, 1, 0, 0, &exp_over->_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			error_display(error_num);
	}
}


