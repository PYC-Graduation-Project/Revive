#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/box_component.h>
#include "object/actor/weapon/weapon.h"

namespace revive
{
	Weapon::Weapon(const std::string& path, const std::string& name)
		:Actor(eMobilityState::kMovable,name)
		,m_mesh_path(path)
	{
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
		m_box_component = CreateSPtr<BoxComponent>();
	}

	bool Weapon::Initialize()
	{
		bool ret = m_static_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_static_mesh_component);
		//ret &= AttachComponent(m_box_component);
		return ret;
	}

	void Weapon::Update(float delta_time)
	{
		Mat4 socket_world_matrix;
		GetSocketMatrix(socket_world_matrix);
		SetPosition(Vec3{ socket_world_matrix._41,socket_world_matrix._42,socket_world_matrix._43 } + m_position_offset);
		SetRotation(m_attached_actor.lock()->GetRotation());
	}

	void const Weapon::GetSocketMatrix(Mat4& out_matrix)
	{
		const auto& skeletal_mesh_component = m_attached_skeletal_mesh_component.lock();
		Mat4 socket_local_matrix = skeletal_mesh_component->GetSocketWorldMatrix(m_socket_name);
		/*Mat4 bone_matrix = mat4::CreateRotationFromQuaternion(skeletal_mesh_component->GetWorldRotation());
		bone_matrix *= mat4::CreateTranslation(skeletal_mesh_component->GetWorldPosition());*/
		out_matrix = socket_local_matrix * m_attached_actor.lock()->GetWorldMatrix();
		//LOG_INFO(out_matrix);
	}

	void Weapon::SetAttachedActor(const SPtr<Actor> actor, const SPtr<SkeletalMeshComponent> skeletal_mesh_component)
	{
		m_attached_actor = actor;
		m_attached_skeletal_mesh_component = skeletal_mesh_component;
	}

	void Weapon::Shutdown()
	{
		m_static_mesh_component = nullptr;
		m_box_component = nullptr;
	}

	//void Weapon::SetSocket()
	//{
	//	const auto& character = m_attached_actor.lock();
	//	Mat4 m_character_matrix = mat4::CreateTranslation(character->GetPosition());
	//	m_character_matrix *= mat4::CreateRotationFromQuaternion(character->GetW);
	//	Mat4 socket_world_matrix = socket_matrix;
	//	//socket_world_matrix *= actor_matrix;

	//	m_static_mesh_component->SetLocalPosition(Vec3{ socket_world_matrix._41, socket_world_matrix._42, socket_world_matrix._43 });
	//	LOG_INFO("{0}", m_static_mesh_component->GetLocalPosition());
	//	Quaternion q;
	//	//XMStoreFloat4(&q, XMQuaternionRotationMatrix(XMLoadFloat4x4(&socket_world_matrix)));
	//	//m_static_mesh_component->SetLocalRotation(q);
	//	//LOG_INFO(m_static_mesh_component->GetWorldMatrix());
	//}

}