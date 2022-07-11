#pragma once
#include <include/client_core.h>
#include <random>
namespace revive_math
{
	using namespace client_fw;

	Vec3 RandomPointInBox(const Vec3& origin, const Vec3& extents)
	{
		Vec3 box_min = origin - extents;
		Vec3 box_max = origin + extents;
		std::random_device rd;
		std::mt19937 mt(rd());

		std::uniform_real_distribution<float> random_box_pointx(box_min.x, box_max.x);
		std::uniform_real_distribution<float> random_box_pointy(box_min.y, box_max.y);
		std::uniform_real_distribution<float> random_box_pointz(box_min.z, box_max.z);

		return Vec3{ random_box_pointx(mt),random_box_pointy(mt),random_box_pointz(mt) };
	}
}
