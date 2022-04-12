#pragma once
#include "object/actor/enemy.h"

namespace client_fw
{
}

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
		virtual bool SetCollisionComponent() override;
		virtual void Attack() override;

		Vec3 m_player_position = vec3::ZERO;

		bool m_is_fire = false;
		int m_animation_select_num = 0;
		std::array<std::string, 6> m_animation_name =
		{
			"appear",
			"attack",
			"death","hit",
			"idle","sprint"
		};

	};
}