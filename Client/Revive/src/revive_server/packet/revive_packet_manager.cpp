
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
	RegisterRecvFunction(SC_PACKET_BASE_STATUS, [this](int c_id, unsigned char* p) {ProcessBaseStatus(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ATTACK, [this](int c_id, unsigned char* p) {ProcessAttack(c_id, p); });
	RegisterRecvFunction(SC_PACKET_STATUS_CHANGE, [this](int c_id, unsigned char* p) {ProcessStatusChange(c_id, p); });
	RegisterRecvFunction(SC_PACKET_WIN, [this](int c_id, unsigned char* p) {ProcessGameWin(c_id, p); });
	RegisterRecvFunction(SC_PACKET_DEFEAT, [this](int c_id, unsigned char* p) {ProcessGameDefeat(c_id, p); });
	RegisterRecvFunction(SC_PACKET_DEAD, [this](int c_id, unsigned char* p) {ProcessDead(c_id, p); });
}
void RevivePacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	//cout<<<< "Packetx :" << move_packet->x << ", y : " << move_packet->y << ", z : " << move_packet->z << endl;
	auto mover = m_obj_map.find(packet->id);
	Vec3 recv_pos{ packet->x,packet->y,packet->z };
	cout << "나 위치:"<<recv_pos << endl;
	//cout << recv_pos << endl;
	if (mover != m_obj_map.end())
	{
		if (mover->second->GetIsActive() == false)return;
		if (isnan(packet->x) || isnan(packet->y) || isnan(packet->z))return;
		auto end_t = std::chrono::system_clock::now();
		

		mover->second->SetPosition(move(recv_pos));

		if (mover->second->m_move_time <= end_t) {
			PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::MoveObjectMessageEventInfo>(HashCode("move object"), mover->second->GetPosition()), packet->id);
			mover->second->m_move_time = end_t + 50ms;
		}
	}
	else
	{
		LOG_INFO("No Object{0}", packet->id);
	}
	
}

void RevivePacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_game_info.SetNetworkID( packet->id);
	LOG_INFO("로그인 성공" );
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::SignInMessageOkEventInfo>(HashCode("sign in")));

}

void RevivePacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "회원가입 성공" << endl;
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr < revive::SignUpMessageEventOkInfo>(HashCode("sign up")));
}

void RevivePacketManager::ProcessLoginFali(int c_id, unsigned char* p)
{
	sc_packet_login_fail* packet = reinterpret_cast<sc_packet_login_fail*>(p);
	//cout << "로그인 실패" << endl;
	eLoginFailType type;
	switch (packet->reason)
	{
	case 1: {
		type = eLoginFailType::kDBError;
		break;
	}
	case 2: {
		type = eLoginFailType::kUserFull;
		break;
	}
	case 3: {
		type = eLoginFailType::kAlreadyLogin;
		break;
	}
	case 4: {
		type = eLoginFailType::kInvalidPW;
		break;
	}
	case 5: {
		type = eLoginFailType::kInvalidID;
		break;
	}
	case 6: {
		type = eLoginFailType::kExistID;
		break;
	}
	}
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::LoginFailMessageEventInfo>(HashCode("login fail"),type));

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
	NW_OBJ_TYPE nw_type;
	//string da = "ss";
	packet->id == m_game_info.GetNetworkID() ? nw_type = NW_OBJ_TYPE::OT_MY_PLAYER : nw_type =(NW_OBJ_TYPE)packet->object_type;
		auto res=m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
			packet->id,
			packet->maxhp,
			packet->name,
			packet->x,
			packet->y,
			packet->z,
			nw_type,
			packet->damage
		));
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::ObjectInfoMessageEventInfo>(HashCode("spawn object"), m_obj_map[packet->id]));
}

void RevivePacketManager::ProcessTime(int c_id, unsigned char* p)
{
	sc_packet_time* packet = reinterpret_cast<sc_packet_time*>(p);
	//float d_ms = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - packet->send_time;
	auto end_t = chrono::system_clock::now().time_since_epoch();
	int t = end_t.count() - packet->send_time;
	//cout << "네트워크 딜레이" << std::chrono::duration_cast<std::chrono::milliseconds>(end_t.count() - packet->send_time) << endl;
	LOG_INFO("네트워크 딜레이:{0}",t);
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
		if (target->second->GetIsActive() == false)return;
	
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::NpcAttackEventInfo>(HashCode("npc attack"),
			target->second->GetPosition()), packet->obj_id);//y값+80해서 보내주기
		

	}
	else if (target == m_obj_map.end()&&packet->target_id == -1)
	{
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::NpcAttackEventInfo>(HashCode("npc attack"),
		Vec3(2400.0f,400.0f,2850.0f)), packet->obj_id);
	}
	else
		LOG_INFO("없는 객체 공격{0}",packet->obj_id);
}

void RevivePacketManager::ProcessAttack(int c_id, unsigned char* p)
{
	sc_packet_attack* packet = reinterpret_cast<sc_packet_attack*>(p);
	auto attacker = m_obj_map.find(packet->obj_id);
	if (attacker != m_obj_map.end())
	{
		if (attacker->second->GetIsActive() == false)return;
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::RecvAttackEventInfo>(HashCode("player attack")), packet->obj_id);
	}
	else
		LOG_INFO("None");

}

void RevivePacketManager::ProcessBaseStatus(int c_id, unsigned char* p)
{
	sc_packet_base_status* packet = reinterpret_cast<sc_packet_base_status*>(p);
	if (m_game_info.GetRoomID() == -1)
		m_game_info.SetRoomID(packet->room_id);
	m_game_info.SetBaseHp(packet->hp);
	//LOG_INFO("BaseHP:{0}", packet->hp);
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::BaseHpChangeEventInfo>(HashCode("base hp change"), packet->hp));
}

void RevivePacketManager::ProcessStatusChange(int c_id, unsigned char* p)
{
	sc_packet_status_change*packet = reinterpret_cast<sc_packet_status_change*>(p);

	auto obj = m_obj_map.find(packet->id);
	if (obj != m_obj_map.end())
	{
		//LOG_INFO("{0}가 맞았다 hp:{1}",obj->second->GetName(),packet->hp);
		if (obj->second->GetIsActive() == false)return;
		obj->second->SetHP(packet->hp);
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::StatusChangeEventInfo>(HashCode("status change"), packet->hp), packet->id );
	}
}

void RevivePacketManager::ProcessGameWin(int c_id, unsigned char* p)
{
	sc_packet_win* packet = reinterpret_cast<sc_packet_win*>(p);
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::GameWinEventInfo>(HashCode("win")));
	Reset();
}

void RevivePacketManager::ProcessGameDefeat(int c_id, unsigned char* p)
{
	sc_packet_defeat* packet = reinterpret_cast<sc_packet_defeat*>(p);
	//m_stop_recv = true;
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::GameWinEventInfo>(HashCode("defeat")));
	Reset();
}

void RevivePacketManager::ProcessDead(int c_id, unsigned char* p)
{
	sc_packet_dead* packet = reinterpret_cast<sc_packet_dead*>(p);
	auto obj = m_obj_map.find(packet->obj_id);
	if (obj != m_obj_map.end())
	{
		obj->second->SetIsActive(false);
	}
	PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::ObjectDeadEventInfo>(HashCode("dead")), packet->obj_id);
	
}

void RevivePacketManager::Reset()
{
	for (auto& obj : m_obj_map)
	{
		if(obj.second->GetIsActive()==true)
			PacketHelper::DisconnectActorFromServer(obj.first);
		obj.second = nullptr;
		
	}
	m_obj_map.clear();
}
