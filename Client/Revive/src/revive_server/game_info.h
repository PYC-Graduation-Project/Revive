#pragma once
class GameInfo {
public:
	GameInfo() {};
	~GameInfo()=default;

	int GetRoomID() { return m_room_id; }
	int GetNetworkID() { return m_network_id; }
	float GetBaseHp() { return m_base_hp; }
	void SetBaseHp(float val) { m_base_hp = val; }
	void SetRoomID(int val) { m_room_id = val; }
	void SetNetworkID(int val) { m_network_id = val; }
	const int BASE_ID = -1;
private:
	int m_room_id=-1;
	int m_network_id=-1;
	float m_base_hp{ BASE_HP };

};