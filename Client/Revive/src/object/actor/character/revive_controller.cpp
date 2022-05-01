#include <include/client_core.h>
#include <client/object/actor/pawn.h>
#include <client/input/input.h>
#include "object/actor/character/revive_player.h"
#include "object/actor/character/revive_controller.h"

namespace revive
{
	ReviveController::ReviveController()
		:PlayerController("revive player controller")
	{
	}
	bool ReviveController::Initialize()
	{
		bool ret = PlayerController::Initialize();

		const auto& player = std::static_pointer_cast<RevivePlayer>(m_controlled_pawn);

		//이동 및 조작
		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this, player](float axis)->bool {
			bool ret = player->GetIsDying();
			if (ret == false)
			{
				m_controlled_pawn->AddMovementInput(GetForward(), axis);
				player->FixYPosition();
			}
			return ret;
			});

		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this, player](float axis)->bool {
			bool ret = player->GetIsDying();
			if (ret == false)
			{
				m_controlled_pawn->AddMovementInput(GetRight(), axis);
				player->FixYPosition();
			}
			return ret; 
			});

		RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
			[this](float axis)->bool {
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				AddYawInput(axis * relative_pos.x);
				//MinPItch();
				return true;
			});

		RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
			[this](float axis)->bool {
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				AddPitchInput(axis * relative_pos.y);
				return true;
			});
		
		return ret;
	}
	void ReviveController::MinPItch()
	{
		Vec3 rot = quat::QuaternionToEuler(GetRotation());
		if (math::ToDegrees(rot.x) < 5) //컨트롤러에서 SetRotation(rot);를 안해주는게 제일 좋은 방법
		{
			rot.x += math::ToRadian(5.0f) - rot.x;
			SetRotation(quat::CreateQuaternionFromRollPitchYaw(rot.x, rot.y, rot.z));
		}
	}
}