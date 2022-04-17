#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class StaticMeshComponent;
	class SkeletalMeshComponent;
	class BoxComponent;
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
		std::string m_mesh_path;
		std::string m_socket_name;

		SPtr<StaticMeshComponent> m_static_mesh_component;
		SPtr<BoxComponent> m_box_component; //Hit Box

		WPtr<Actor> m_attached_actor;
		WPtr<SkeletalMeshComponent> m_attached_skeletal_mesh_component;

		Vec3 m_position_offset = vec3::ZERO;
		Quaternion m_rotation_offset = Quaternion{ 0.f,0.f,0.f,1.f };

		void const GetSocketMatrix(Mat4& out_matrix);
	public:
		//void AttachSkeletalMeshComponent(const SPtr<SkeletalMeshComponent> skeletal_mesh_component);
		void SetAttachedActor(const SPtr<Actor> actor, const SPtr<SkeletalMeshComponent> skeletal_mesh_component);
		void SetSocketName(const std::string& socket_name) { m_socket_name = socket_name; }
		//void SetSocket(const Mat4& socket_matrix, const Mat4& actor_matrix);
	};
}