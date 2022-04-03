#include "pch.h"
#include "Astar.h"

#include<iostream>



bool Astar::SearchAllPath(char* map[], Vec2 src, Vec2 dst)
{
	if (false == IsInRange(dst))return false;
	if (true == IsBlocked(dst))return false;

	Node start_node = testing_map[src.first][src.second];
	start_node.pos = src;
	start_node.parant_pos = src;
	start_node.f = 0;
	start_node.g = 0;
	start_node.h = GethValue(src.first, src.second, dst);
	open_pq.push(start_node);

	while (!open_pq.empty())
	{
		Node now = open_pq.top();
		open_pq.pop();

		if (now.pos == dst)
		{
			TracePath(now, src, dst);
			return true;
		}
		if (close_set.find(now.pos) != close_set.end())
			continue;
		int x = now.pos.second;
		int y = now.pos.first;

		close_set.insert(now.pos);
		for (int i = 0; i < 8; ++i)
		{
			int nx = now.pos.second + dirX[i];
			int ny = now.pos.first + dirY[i];
			Vec2 new_pos{ ny,nx };
			if (false == IsInRange(new_pos))continue;
			if (true == IsBlocked(new_pos))continue;
			if (close_set.find(new_pos) != close_set.end())continue;
			int ng = now.g + cost[i];
			int nh = GethValue(ny, nx, dst);
			int nf = ng + nh;
			if (testing_map[ny][nx].f < nf)
				continue;
			testing_map[ny][nx].f = nf;
			testing_map[ny][nx].g = ng;
			testing_map[ny][nx].h = nh;
			testing_map[ny][nx].pos = new_pos;
			testing_map[ny][nx].parant_pos = now.pos;
			open_pq.push(testing_map[ny][nx]);

		}

	}

	return false;
}

bool Astar::IsBlocked(Vec2 dst)
{
	return (test_map[dst.first][dst.second] == '0');
}

bool Astar::IsInRange(Vec2 pos)
{

	return(pos.first >= 0 && pos.first < COL&& pos.second >= 0 && pos.second < ROW);
}

void Astar::TracePath(Node& now, Vec2 src, Vec2 dst)
{
	Node node = now;
	std::cout << "µé¾î¿È" << std::endl;
	while (testing_map[node.pos.first][node.pos.second].parant_pos != src)
	{
		test_map[node.parant_pos.first][node.parant_pos.second] = '*';
		node = testing_map[node.parant_pos.first][node.parant_pos.second];
	}

}
