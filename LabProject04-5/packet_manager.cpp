#include "stdafx.h"
#include "packet_manager.h"
#include<iostream>
using namespace std;

PacketManager::PacketManager()
{
}

PacketManager::~PacketManager()
{
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type)
	{
	case SC_PACKET_LOGIN_OK:
	{
		ProcessLogin(c_id, p);
	}
	case SC_PACKET_MOVE: {
		ProcessMove(c_id, p);
	}
	}
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* move_packet = reinterpret_cast<sc_packet_move*>(p);
	//pos = XMFLOAT3(move_packet->x, move_packet->y, move_packet->z);
	//look = XMFLOAT3(move_packet->look_x, move_packet->look_y, move_packet->look_z);
	//right = XMFLOAT3(move_packet->right_x, move_packet->right_y, move_packet->right_z);
	cout << "Packetx :" << move_packet->x << ", y : " << move_packet->y << ", z : " << move_packet->z << endl;
}

void PacketManager::ProcessLogin(int c_id, unsigned char* p)
{
	sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);

}
