
#pragma once

#include<queue>
#include<functional>
#include<set>
#include<vector>
#include<bitset>
#include"vec3.h"
const int ROW = 40;
const int COL = 40;
//한칸에 몇미터? 
//타일기준으로 하기
//현재 위치에서 제일가까운 타일 찾아주기
//60x60이 국룰 시작을 30,30으로 배치->40x40으로 변경 20,20으로 배치
using Vec2 = std::pair<int, int>;
//using CloseList = std::pair < std::bitset<ROW>, std::bitset<COL>>;
class MapObj;
class Node {
public:
	Node() {
		g = 9999;
		h = 9999;
		f = g + h;
		parant_pos = std::make_pair(-1, -1);
	}
	Node(Vec2& pos, Vec2& parant_pos, int g, int h) :pos(pos), parant_pos(parant_pos),
		g(g), h(h)
	{
		f = g + h;

	}
	~Node() = default;

	int f, g, h;
	Vec2 pos;
	Vec2 parant_pos;

	bool operator>(const Node& a) const {
		return f > a.f;
	}
	bool operator<(const Node& a) const {
		if (f == a.f)
			return g < a.g;
		return f < a.f;
	}
	bool operator ==(const Node& a)const {
		return pos == a.pos;
	}
};


class Astar
{
public:
	Astar() {
		m_start_pos = Vec2(ROW / 2, COL / 2);
	};
	~Astar() {};

	bool SearchAllPath(std::vector<MapObj>& map_objects, const Vector3& src, const Vector3& dst);
	bool IsBlocked(std::vector<MapObj>& map_objects,Vec2 dst);
	bool IsInRange(std::vector<MapObj>& map_objects,Vec2 pos);
	
	int GethValue(int row, int col, Vec2 dst) {
		return std::sqrt(std::pow(row - dst.first, 2) + std::pow(col - dst.second, 2));
	}
	bool isDestination(int row, int col, Vec2 dst) {
		if (row == dst.first && col == dst.second) return true;
		return false;
	}
	void TracePath(Node& now, Vec2 src, Vec2 dst);
private:
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>>open_pq;
	Node testing_map[COL][ROW];
	std::set<Vec2>close_set;
	Vec2 m_start_pos;
	std::vector<Vec2>result_vec;
	int dirX[8] = { -1,0,1,0,-1,1,1,-1 };
	int dirY[8] = { 0,-1,0,1,-1,-1,1,1 };
	int cost[8] = { 10,10,10,10,14,14,14,14 };
};

