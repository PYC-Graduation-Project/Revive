#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class StaticMeshComponent;
	class SkeletalMeshComponent;
	class BoxComponent;
	struct CollisionInfo;
}

namespace revive
{
	using namespace client_fw;

	class Weapon : public Actor
	{
	public:
		Weapon(const std::string& path, const std::string& name = "weapon");
		virtual ~Weapon() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	protected:
		int m_owner_network_id = 9999;

		bool m_is_collision = false;

		std::string m_mesh_path;
		std::string m_socket_name;

		SPtr<StaticMeshComponent> m_static_mesh_component;
		SPtr<BoxComponent> m_box_component; //Hit Box

		WPtr<Actor> m_attached_actor;
		WPtr<SkeletalMeshComponent> m_attached_skeletal_mesh_component;

		Vec3 m_position_offset = vec3::ZERO;
		Vec3 m_rotation_offset = vec3::ZERO;

		//void const GetSocketMatrix(Mat4& out_matrix);
	public:
		void SetPositionOffset(const Vec3& position_offset) { m_position_offset = position_offset; }
		void SetRotationOffset(const Vec3& rotation_offset) { m_rotation_offset = rotation_offset; }
		void SetAttachedActor(const SPtr<Actor> actor, const SPtr<SkeletalMeshComponent> skeletal_mesh_component);
		void SetSocketName(const std::string& socket_name) { m_socket_name = socket_name; }
		const bool GetIsCollision() const { return m_is_collision; }
		void SetIsCollision(const bool value) { m_is_collision = value; }
		void SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collision_event);
		const CollisionInfo& GetCollisionInfo() const;

		void SetOwnerNetworkID(int owner_nw_id) { m_owner_network_id = owner_nw_id; }
		const int GetOwnerNetworkID() const { return m_owner_network_id; }
		//void SetSocket(const Mat4& socket_matrix, const Mat4& actor_matrix);
	};
}