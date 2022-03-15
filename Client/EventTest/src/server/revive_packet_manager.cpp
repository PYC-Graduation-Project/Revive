
#include "stdafx.h"

#include"server/network_obj_manager.h"
#include"server/network_move_object.h"
#include "revive_packet_manager.h"

using namespace std;
void RevivePacketManager::Init()
{
	RegisterRecvFunction(SC_PACKET_MOVE, [this](int c_id, unsigned char* p) {ProcessMove(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_IN_OK, [this](int c_id, unsigned char* p) {ProcessSignIn(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_UP_OK, [this](int c_id, unsigned char* p) {ProcessSignUp(c_id, p); });
	RegisterRecvFunction(SC_PACKET_MATCHING, [this](int c_id, unsigned char* p) {ProcessMatching(c_id, p); });
	RegisterRecvFunction(SC_PACKET_LOGIN_FAIL, [this](int c_id, unsigned char* p) {ProcessLoginFali(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO, [this](int c_id, unsigned char* p) {ProcessObjInfo(c_id, p); });
	RegisterRecvFunction(SC_PACKET_TIME, [this](int c_id, unsigned char* p) {ProcessTime(c_id, p); });

}
void RevivePacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	//cout << "Packetx :" << move_packet->x << ", y : " << move_packet->y << ", z : " << move_packet->z << endl;
	NetworkMoveObj* mover = static_cast<NetworkMoveObj*>(NetworkObjManager::GetInst()->GetObj(packet->id));
	if (!mover) {
		cout << "없는 객체입니다!" << endl;
		return;
	}
	client_fw::Vec3 pos{ packet->x,packet->y ,packet->z };
	client_fw::Quaternion rot{ packet->r_x,packet->r_y ,packet->r_z ,packet->r_w };
	mover->SetPosition(pos);
	mover->SetRotation(rot);

}

void RevivePacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_id = packet->id;
	cout << "로그인 성공" << endl;


}

void RevivePacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "회원가입 성공" << endl;
}

void RevivePacketManager::ProcessLoginFali(int c_id, unsigned char* p)
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

void RevivePacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "매칭성사" << endl;
}

void RevivePacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	if (packet->object_type == static_cast<char>(NW_OBJ_TYPE::OT_BASE))//기지를 어떻게 처리할지 좀더 고민..
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

void RevivePacketManager::ProcessTime(int c_id, unsigned char* p)
{
	sc_packet_time* packet = reinterpret_cast<sc_packet_time*>(p);
	//float d_ms = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - packet->send_time;
	LOG_INFO(packet->time);
	//LOG_INFO( d_ms);//여기부분 일단 두자 네트워크 딜레이 측정
}