
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
	
	PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::MoveObjectMessageEventInfo>(HashCode("move object"), recv_pos,recv_rot),packet->id);
}

void RevivePacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_game_info.SetNetworkID( packet->id);
	LOG_INFO("�α��� ����" );
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::SignInMessageOkEventInfo>(HashCode("sign in")));

}

void RevivePacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	sc_packet_sign_up_ok* packet = reinterpret_cast<sc_packet_sign_up_ok*>(p);
	cout << "ȸ������ ����" << endl;
}

void RevivePacketManager::ProcessLoginFali(int c_id, unsigned char* p)
{
	sc_packet_login_fail* packet = reinterpret_cast<sc_packet_login_fail*>(p);
	//cout << "�α��� ����" << endl;
	switch (packet->reason)
	{
	case 1: {
		LOG_INFO( "DBError");
		break;
	}
	case 2: {
		LOG_INFO("����� Full");
		break;
	}
	case 3: {
		LOG_INFO("�̹� ������");
		break;
	}
	case 4: {
		LOG_INFO("���Ʋ��" );
		break;
	}
	case 5: {
		LOG_INFO("���̵����");
		break;
	}
	case 6: {
		LOG_INFO("�ش� ���̵� ����");
		break;
	}
	}
}

void RevivePacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	sc_packet_matching* packet = reinterpret_cast<sc_packet_matching*>(p);
	cout << "��Ī����" << endl;
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::MatchingMessageOKEventInfo>(HashCode("match")));
}

void RevivePacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	Network::matching_end = true;
	NW_OBJ_TYPE nw_type;
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
	LOG_INFO(packet->time);
	//LOG_INFO( d_ms);//����κ� �ϴ� ���� ��Ʈ��ũ ������ ����
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

	}
	else if (target == m_obj_map.end()&&packet->target_id == -1)
	{
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::NpcAttackEventInfo>(HashCode("npc attack"),
		Vec3(2400.0f,300.0f,2850.0f)), packet->obj_id);
	}
	else
		LOG_INFO("���� ��ü ����");
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

void RevivePacketManager::ProcessBaseStatus(int c_id, unsigned char* p)
{
	sc_packet_base_status* packet = reinterpret_cast<sc_packet_base_status*>(p);
	if (m_game_info.GetRoomID() == -1)
		m_game_info.SetRoomID(packet->room_id);
	m_game_info.SetBaseHp(packet->hp);
	LOG_INFO("BaseHP:{0}", packet->hp);
	PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::BaseHpChangeEventInfo>(HashCode("base hp change"), packet->hp));
}

void RevivePacketManager::ProcessStatusChange(int c_id, unsigned char* p)
{
	sc_packet_status_change*packet = reinterpret_cast<sc_packet_status_change*>(p);
	auto obj = m_obj_map.find(packet->id);
	if (obj != m_obj_map.end())
	{
		LOG_INFO("{0}�� �¾Ҵ� hp:{1}",packet->id,packet->hp);
		obj->second->SetHP(packet->hp);
		PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::StatusChangeEventInfo>(HashCode("status change"), packet->hp), packet->id );
	}
}
