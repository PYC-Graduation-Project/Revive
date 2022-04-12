#pragma once
#include "object/actor/projectile.h"

namespace revive
{
	//static mesh, sphere component �����ؾ���
	class Stone : public Projectile
	{
	public:
		Stone(const std::string& name = "Stone");
		virtual ~Stone() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
	
	};
}