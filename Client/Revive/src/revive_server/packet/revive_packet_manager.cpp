
#include "stdafx.h"
#include"server/network.h"
#include"server/network_obj_manager.h"
#include"server/network_move_object.h"
#include "revive_packet_manager.h"
#include <client/event/packetevent/packet_helper.h>
#include"revive_server/message/message_event_info.h"
using namespace std;
using namespace client_fw;
void RevivePacketManager::Init()
{
	m_obj_map = unordered_map<int, client_fw::SPtr<NetworkMoveObj>>();
	RegisterRecvFunction(SC_PACKET_MOVE, [this](int c_id, unsigned char* p) {ProcessMove(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_IN_OK, [this](int c_id, unsigned char* p) {ProcessSignIn(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_UP_OK, [this](int c_id, unsigned char* p) {ProcessSignUp(c_id, p); });
	RegisterRecvFunction(SC_PACKET_MATCHING, [this](int c_id, unsigned char* p) {ProcessMatching(c_id, p); });
	RegisterRecvFunction(SC_PACKET_LOGIN_FAIL, [this](int c_id, unsigned char* p) {ProcessLoginFali(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO, [this](int c_id, unsigned char* p) {ProcessObjInfo(c_id, p); });
	RegisterRecvFunction(SC_PACKET_TIME, [this](int c_id, unsigned char* p) {ProcessTime(c_id, p); });
	RegisterRecvFunction(SC_PACKET_TEST, [this](int c_id, unsigned char* p) {ProcessTest(c_id, p); });
	RegisterRecvFunction(SC_PACKET_NPC_ATTACK, [this](int c_id, unsigned char* p) {ProcessNpcAttack(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ATTACK, [this](int c_id, unsigned char* p) {ProcessAttack(c_id, p); });
}
void RevivePacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	//cout << "Packetx :" << move_packet->x << ", y : " << move_packet->y << ", z : " << move_packet->z << endl;
	auto mover = m_obj_map.find(packet->id);
	Vec3 recv_pos{ packet->x,packet->y,packet->z };
	Quaternion recv_rot{ packet->r_x,packet->r_y,packet->r_z,packet->r_w };
	if (mover != m_obj_map.end())
	{
		mover->second->SetPosition(move(recv_pos));
		mover->second->SetRotation(move(recv_rot));
	}
	else
	{
		LOG_INFO("No Object");
	}
	//if (!mover) {
	//	cout << "없는 객체입니다!" << endl;
	//	return;
	//}
	//client_fw::Vec3 pos{ packet->x,packet->y ,packet->z };
	//client_fw::Quaternion rot{ packet->r_x,packet->r_y ,packet->r_z ,packet->r_w };
	//mover->SetPosition(pos);
	//mover->SetRotation(rot);
	//std::cout << recv_pos << std::endl;
	PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::MoveObjectMessageEventInfo>(HashCode("move object"), recv_pos,recv_rot),packet->id);
}

void RevivePacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_id = packet->id;
	LOG_INFO("로그인 성공" );


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
		LOG_INFO( "DBError");
		break;
	}
	case 2: {
		LOG_INFO("사용자 Full");
		break;
	}
	case 3: {
		LOG_INFO("이미 접속중");
		break;
	}
	case 4: {
		LOG_INFO("비번틀림" );
		break;
	}
	case 5: {
		LOG_INFO("아이디없음");
		break;
	}
	case 6: {
		LOG_INFO("해당 아이디 존재");
		break;
	}
	}
}

void RevivePacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	sc_packet_matching* packet = reinterpret_cast<sc_packet_matching*>(p);
	cout << "매칭성사" << endl;
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::MatchingMessageOKEventInfo>(HashCode("match")));
}

void RevivePacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	Network::matching_end = true;
	if (packet->object_type == static_cast<char>(NW_OBJ_TYPE::OT_BASE))//기지를 어떻게 처리할지 좀더 고민..
	{
		//obj = new NetworkObj(
		//	packet->id,
		//	packet->maxhp,
		//	packet->name,
		//	packet->x,
		//	packet->y,
		//	packet->z,
		//	(NW_OBJ_TYPE)packet->object_type
		//);
		//NetworkObjManager::GetInst()->AddObj(packet->id, obj);
	}
	else
	{
		auto res=m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
			packet->id,
			packet->maxhp,
			packet->name,
			packet->x,
			packet->y,
			packet->z,
			(NW_OBJ_TYPE)packet->object_type,
			packet->damage
		));
	}
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::ObjectInfoMessageEventInfo>(HashCode("spawn object"), m_obj_map[packet->id]));
}

void RevivePacketManager::ProcessTime(int c_id, unsigned char* p)
{
	sc_packet_time* packet = reinterpret_cast<sc_packet_time*>(p);
	//float d_ms = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - packet->send_time;
	LOG_INFO(packet->time);
	//LOG_INFO( d_ms);//여기부분 일단 두자 네트워크 딜레이 측정
}

void RevivePacketManager::ProcessTest(int c_id, unsigned char* p)
{
	sc_packet_test* packet = reinterpret_cast<sc_packet_test*>(p);
	PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::TestMessageEventInfo>(HashCode("testmove"), packet->speed, packet->x, packet->y, packet->z),packet->id);
	Vec3 recv_pos{ packet->x,packet->y,packet->z };
	
	
}

void RevivePacketManager::ProcessNpcAttack(int c_id, unsigned char* p)
{
	sc_packet_npc_attack*packet= reinterpret_cast<sc_packet_npc_attack*>(p);
	auto target = m_obj_map.find(packet->target_id);
	if (target != m_obj_map.end()) {
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::NpcAttackEventInfo>(HashCode("npc attack"),
			target->second->GetPosition()), packet->obj_id);
		LOG_INFO(target->second->GetPosition());
	}
	else
		LOG_INFO("없는 객체 공격");
}

void RevivePacketManager::ProcessAttack(int c_id, unsigned char* p)
{
	sc_packet_attack* packet = reinterpret_cast<sc_packet_attack*>(p);
	auto attacker = m_obj_map.find(packet->obj_id);
	if (attacker != m_obj_map.end())
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::RecvAttackEventInfo>(HashCode("player attack")), packet->obj_id);
	else
		LOG_INFO("None");
}
