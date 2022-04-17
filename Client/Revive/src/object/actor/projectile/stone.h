#pragma once
#include "object/actor/projectile/projectile.h"

namespace revive
{
	//static mesh, sphere component 변경해야함
	class Stone : public Projectile
	{
	public:
		Stone(const std::string& name = "Stone");
		virtual ~Stone() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

		void SetMeshLocalPosition(const Vec3& position);
	};
}