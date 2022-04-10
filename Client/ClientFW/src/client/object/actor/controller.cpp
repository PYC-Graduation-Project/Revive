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
		if (math::NearZero(m_yaw_speed) == false)
		{
			UpdateYaw(delta_time);
		}
		if (math::NearZero(m_pitch_speed) == false)
		{
			UpdatePitch(delta_time);
		}
		if (math::NearZero(m_roll_speed) == false)
		{
			UpdateRoll(delta_time);
		}

		m_pitch_speed = m_yaw_speed = m_roll_speed = 0.0f;
	}

	void Controller::UpdateYaw(float delta_time)
	{
		Quaternion rot_value = quat::CreateQuaternionFromNormal(vec3::AXIS_Y, m_yaw_speed * delta_time);
		SetRotation(GetRotation() * rot_value);
		if (m_controlled_pawn != nullptr)
		{
			if (m_controlled_pawn->IsUseControllerYaw())
				m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation() * rot_value);
			else
				m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation());
		}
	}

	void Controller::UpdatePitch(float delta_time)
	{
		Vec3 forward = GetForward();
		forward.x = 0.0f;
		forward.Normalize();
		Quaternion rot = GetRotation();

		auto Rotate([this, &rot, &forward, delta_time]()->Quaternion
			{
				Quaternion rot_value = quat::CreateQuaternionFromNormal(GetRight(), m_pitch_speed * delta_time);
				rot *= rot_value;
				forward = vec3::TransformNormal(vec3::AXIS_Z, rot);
				forward.x = 0.0f;
				forward.Normalize();
				return rot_value;
			});

		auto SetPitch([this](const Quaternion& rot, const Quaternion& rot_value)
			{	
				SetRotation(rot);
				if (m_controlled_pawn != nullptr)
				{
					if (m_controlled_pawn->IsUseControllerPitch())
						m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation() * rot_value);
					else
						m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation());
				}
			});
		
		if (math::NearZero(forward.z) == false)
		{
			if (forward.z > 0.0f)
			{
				Quaternion rot_value = Rotate();
				if (forward.z > 0.0f && math::NearZero(math::ToRadian(90.0f) - vec3::BetweenAngle(vec3::AXIS_Z, forward)) == false)
					SetPitch(rot, rot_value);
			}
			else if (forward.z < 0.0f)
			{
				Quaternion rot_value = Rotate();
				if (forward.z < 0.0f && math::NearZero(math::ToRadian(90.0f) - vec3::BetweenAngle(vec3::NEG_AXIS_Z, forward)) == false)
					SetPitch(rot, rot_value);
			}
		}
	}

	void Controller::UpdateRoll(float delta_time)
	{
		Vec3 right = GetRight();
		right.z = 0.0f;
		right.Normalize();
		Quaternion rot = GetRotation();

		auto Rotate([this, &rot, &right, delta_time]()->Quaternion
			{
				Quaternion rot_value = quat::CreateQuaternionFromNormal(GetForward(), m_roll_speed * delta_time);
				rot *= rot_value;
				right = vec3::TransformNormal(vec3::AXIS_X, rot);
				right.z = 0.0f;
				right.Normalize();
				return rot_value;
			});

		auto SetRoll([this](const Quaternion& rot, const Quaternion& rot_value)
			{
				SetRotation(rot);
				if (m_controlled_pawn != nullptr)
				{
					if (m_controlled_pawn->IsUseControllerRoll())
						m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation() * rot_value);
					else
						m_controlled_pawn->SetRotation(m_controlled_pawn->GetRotation());
				}
			});


		if (math::NearZero(right.x) == false)
		{
			if (right.x > 0.0f)
			{
				Quaternion rot_value = Rotate();
				if (right.x > 0.0f && math::NearZero(math::ToRadian(90.0f) - vec3::BetweenAngle(vec3::AXIS_X, right)) == false)
					SetRoll(rot, rot_value);
			}
			else if (right.x < 0.0f)
			{
				Quaternion rot_value = Rotate();
				if (right.x < 0.0f && math::NearZero(math::ToRadian(90.0f) - vec3::BetweenAngle(vec3::NEG_AXIS_X, right)) == false)
					SetRoll(rot, rot_value);
			}
		}
	}

	void Controller::Possess(const SPtr<Pawn>& pawn)
	{
		m_controlled_pawn = pawn;
		pawn->SetController(std::static_pointer_cast<Controller>(shared_from_this()));
		SetRotation(m_controlled_pawn->GetRotation());
	}

	void Controller::UnPossess()
	{
		m_controlled_pawn = nullptr;
	}

}
