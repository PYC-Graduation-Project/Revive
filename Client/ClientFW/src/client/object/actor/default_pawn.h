#pragma once
#include "client/object/actor/pawn.h"

namespace client_fw
{
	class StaticMeshComponent;
	class MovementComponent;

	class DefaultPawn : public Pawn
	{
	public:
		DefaultPawn(const std::string& name = "default pawn");
		virtual ~DefaultPawn() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void AddMovementInput(const Vec3& direction, float scale) override;

	protected:
		SPtr<MovementComponent> m_movement_component;
	};
}



