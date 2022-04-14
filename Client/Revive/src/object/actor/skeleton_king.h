#pragma once
#include "object/actor/enemy.h"

namespace revive
{
	class SkeletonKing : public Enemy
	{
	public:
		SkeletonKing(const std::string& name = "Skeleton King");
		virtual ~SkeletonKing() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	private:
		virtual bool SetCollisionComponent() override;
		virtual void Attack() override;

		SPtr<BoxComponent> m_weapon_collision_box; //юс╫ц

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