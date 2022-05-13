#pragma once
#include "object/actor/character/enemy.h"

namespace revive
{
	class Axe;

	class SkeletonKing : public Enemy
	{
	public:
		SkeletonKing(const std::string& name = "Skeleton King");
		virtual ~SkeletonKing() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	private:
		virtual bool SetCollisionComponent() override;
		virtual void Attack() override;

		int m_animation_select_num = 0;
		std::array<std::string, 6> m_animation_name =
		{
			"appear",
			"attack",
			"death","hit",
			"idle","run",
		};
	};
}