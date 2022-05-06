#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SkeletalMeshComponent;
	class StaticMeshComponent;
}

namespace anim_test
{
	using namespace client_fw;

	class TestActor : public Actor
	{
	public:
		TestActor(eMobilityState mobility, const std::string& static_mesh_path, const std::string& animation_name);
		virtual ~TestActor() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

		void SetAnimation(const std::string& animation_name);

		void SetWeapon(const std::string& mesh_path,const std::string& socket_name);
		void SetWeaponOffset(const Vec3& offset);

		SPtr<SkeletalMeshComponent> GetSkeletalMeshComponent() const;
	protected:
		bool m_is_eqquiped = false;
		std::string m_mesh_path;
		std::string m_socket_name;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<StaticMeshComponent> m_static_mesh_component;

		Vec3 m_weapon_offset = vec3::ZERO;
		
		std::string m_animation_name;
	};
}