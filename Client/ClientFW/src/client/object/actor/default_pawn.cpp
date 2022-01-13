#include "stdafx.h"
#include "client/object/actor/default_pawn.h"
#include "client/object/component/util/simple_movement_component.h"
#include "client/input/input.h"

namespace client_fw
{
	DefaultPawn::DefaultPawn(const std::string& name)
	{
		m_movement_component = CreateSPtr<SimpleMovementComponent>();
	}

	bool DefaultPawn::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);

		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetForward(), axis); return true; });
		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetRight(), axis); return true; });
		RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetUp(), axis); return true; });

		return ret;
	}

	void DefaultPawn::Shutdown()
	{
		m_movement_component = nullptr;
		Pawn::Shutdown();
	}

	void DefaultPawn::AddMovementInput(const Vec3& direction, float scale)
	{
		m_movement_component->AddInputVector(direction * scale);
	}
}
