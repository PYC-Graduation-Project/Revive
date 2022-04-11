#pragma once
#include "object/actor/enemy.h"

namespace revive
{
	class SkeletonSoldier : public Enemy
	{
	public:
		SkeletonSoldier(const std::string& name = "Skeleton Soldier");
		virtual ~SkeletonSoldier() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		//virtual void Update(float delta_time) override;
	private:
		int m_animation_select_num = 0;
		std::array<std::string, 8> m_animation_name =
		{
			"appear",
			"attack(bow)","attack(one_hand_sword)",
			"death","hit",
			"idle","run",
			"sprint"
		};
	};
}