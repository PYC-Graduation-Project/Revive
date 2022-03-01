#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>

//Pawn�� ����ϴ� CharacterŬ���� �߰��ٶ�
namespace anim_test
{
	using namespace client_fw;

	class TestActor : public Actor
	{
	public:
		TestActor(eMobilityState mobility, const std::string& static_mesh_path);
		virtual ~TestActor() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	protected:
		std::string m_mesh_path;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;

	};
}