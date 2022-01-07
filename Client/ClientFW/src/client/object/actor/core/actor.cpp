#include "stdafx.h"
#include "client/object/actor/core/actor.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/component/core/component_manager.h"
#include "client/input/input.h"

namespace client_fw
{
	Actor::Actor(eMobilityState mobility, const std::string& name)
		: m_name(name), m_actor_state(eActorState::kActive)
		, m_mobility_state(mobility)
		, m_position(vec3::ZERO), m_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
		m_component_manager = CreateUPtr<ComponentManager>();
	}

	Actor::~Actor()
	{
	}

	void Actor::InitializeActor()
	{
		Initialize();
	}

	void Actor::ShutdownActor()
	{
		for (auto name : m_registered_input_event)
			Input::UnregisterInputEvent(name);

		m_component_manager->Shutdown();

		Shutdown();
	}

	void Actor::UpdateActor(float delta_time)
	{
		m_is_updated_world_matrix = false;

		UpdateWorldMatrix();

		m_component_manager->Update(delta_time);
		Update(delta_time);

		UpdateWorldMatrix();
	}

	void Actor::UpdateWorldMatrix()
	{
		if (m_update_world_matrix)
		{
			m_world_matrix = mat4::CreateScale(m_scale);
			m_world_matrix *= mat4::CreateRotationFromQuaternion(m_rotation);
			m_world_matrix *= mat4::CreateTranslation(m_position);

			m_is_updated_world_matrix = true;
			m_update_world_matrix = false;
		}
	}
	
	void Actor::SpawnActor(const SPtr<Actor>& actor)
	{
		LevelManager::GetLevelManager().SpawnActor(actor);
	}

	void Actor::AttachComponent(const SPtr<Component> comp)
	{
		m_component_manager->RegisterComponent(comp);
	}

	void Actor::DetachComponent(const SPtr<Component> comp)
	{
		m_component_manager->UnregisterComponent(comp);
	}

	void Actor::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Actor::RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterReleasedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Actor::RegisterAxisEvent(std::string_view name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		if (Input::RegisterAxisEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Actor::RegisterInputEvent(std::string_view name)
	{
		m_registered_input_event.emplace_back(std::move(name));
	}

	void Actor::SetPosition(const Vec3& pos)
	{
		m_position = pos;
		m_update_world_matrix = true;
	}

	void Actor::SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
		m_update_world_matrix = true;
	}

	void Actor::SetRotation(float pitch, float yaw, float roll)
	{
		m_rotation = quat::CreateQuaternionFromRollPitchYaw(pitch, yaw, roll);
		m_update_world_matrix = true;
	}

	void Actor::SetScale(const Vec3& scale)
	{
		m_scale = scale;
		m_update_world_matrix = true;
	}

	void Actor::SetScale(float scale)
	{
		m_scale = Vec3(scale, scale, scale);
		m_update_world_matrix = true;
	}

}