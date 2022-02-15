#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>

//원래는 Pawn을 상속하는 Character클래스를 만들어야하나
//렌더링결과를 확인하기위해 임시로 작성함
namespace anim_test
{
	using namespace client_fw;
	//class SkeletalMeshComponent;

	class TestActor : public Actor
	{
	public:
		TestActor(eMobilityState mobility, const std::string& static_mesh_path);
		virtual ~TestActor() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		std::string m_mesh_path;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
	};
}