#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/component/mesh/static_mesh_component.h>

namespace revive
{
	using namespace client_fw;

	class Ground : public Actor
	{
	public:
		Ground(const std::vector<SPtr<StaticMeshComponent>>& components);
		virtual ~Ground() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		std::vector<SPtr<StaticMeshComponent>> m_static_mesh_components;
	};
}