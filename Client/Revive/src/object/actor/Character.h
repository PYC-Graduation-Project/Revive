#pragma once

#include <client/object/actor/pawn.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/component/core/scene_component.h>

namespace revive
{
	using namespace client_fw;
	
	class Character : public Pawn
	{

	};
	class DefaultCharacter : public DefaultPawn
	{
	public:
		DefaultCharacter(const std::string& name = "default character");
		virtual ~DefaultCharacter() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		void CollisionResponse(const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_component);

	private:
		using Pawn::SetUseControllerPitch;
		using Pawn::SetUseControllerYaw;
		using Pawn::SetUseControllerRoll;
	};
}