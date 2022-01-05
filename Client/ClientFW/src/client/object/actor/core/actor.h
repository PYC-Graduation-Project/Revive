#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	struct EventKeyInfo;
	struct AxisEventKeyInfo;

	enum class eActorState
	{
		kActive, kPaused, kDead
	};

	enum class eMobilityState
	{
		kStatic, kMovable
	};

	class Actor : public IBaseObject
	{
	public:
		Actor(eMobilityState mobility = eMobilityState::kStatic, const std::string& name = "Actor");
		virtual ~Actor();

		void InitializeActor();
		virtual void Initialize() override {}

		void ShutdownActor();
		virtual void Shutdown() override {}

		void UpdateActor(float delta_time);
		virtual void Update(float delta_time) override {}

	protected:
		virtual void RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterAxisEvent(std::string_view name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

		void SpawnActor(const SPtr<Actor>& actor);

	private:
		void UpdateWorldMatrix();

		void RegisterInputEvent(std::string_view name);

	protected:
		std::string m_name;
		eActorState m_actor_state;
		eMobilityState m_mobility_state;

	private:
		std::vector<std::string_view> m_registered_input_event;
		Mat4 m_world_matrix;
		Vec3 m_position;
		Quaternion m_rotation;
		Vec3 m_scale;
		bool m_update_world_matrix = true;
		bool m_is_updated_world_matrix = false;

	public:
		std::string_view GetName() const { return m_name; }
		eActorState GetActorState() const { return m_actor_state; }
		void SetActorState(eActorState actor_state) { m_actor_state = actor_state; }
		eMobilityState GetMobilityState() const { return m_mobility_state; }

		const Mat4& GetWorldMatrix() const { return m_world_matrix; }
		const Vec3& GetPosition() const { return m_position; }
		void SetPosition(const Vec3& pos);
		const Quaternion& GetRotation() const { return m_rotation; }
		void SetRotation(const Quaternion& rot);
		void SetRotation(float pitch, float yaw, float roll);
		const Vec3& GetScale() const { return m_scale; }
		void SetScale(const Vec3& scale);
		void SetScale(float scale);
		bool IsUpdatedWorldMatrix() const { return m_is_updated_world_matrix; }

		Vec3 GetForward() const { return vec3::TransformNormal(vec3::AXIS_Z, m_rotation); }
		Vec3 GetRight() const { return vec3::TransformNormal(vec3::AXIS_X, m_rotation); }
		Vec3 GetUp() const { return vec3::TransformNormal(vec3::AXIS_Y, m_rotation); }
	};
}


