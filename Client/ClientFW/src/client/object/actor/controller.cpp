#include "stdafx.h"
#include "client/object/actor/controller.h"
#include "client/object/actor/pawn.h"

namespace client_fw
{
	Controller::Controller(const std::string& name)
		: Actor(eMobilityState::kMovable, name)
	{
	}

	void Controller::Update(float delta_time)
	{
		m_updated = true;
		if (m_controlled_pawn != nullptr && m_controlled_pawn->IsNeedUpdate())
		{
			m_controlled_pawn->UpdateActor(delta_time);
		}
	}

	void Controller::Possess(const SPtr<Pawn>& pawn)
	{
		m_controlled_pawn = pawn;
		pawn->m_controller = std::static_pointer_cast<Controller>(shared_from_this());
	}

	void Controller::UnPossess()
	{
		m_controlled_pawn = nullptr;
	}

}
