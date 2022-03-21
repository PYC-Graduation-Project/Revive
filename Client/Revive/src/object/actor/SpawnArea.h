#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/component/render/box_component.h>

namespace revive
{
	using namespace client_fw;

	class SpawnArea : public Actor
	{
	public:
		SpawnArea(const std::vector<SPtr<BoxComponent>>& box_components);

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		std::vector<SPtr<BoxComponent>> m_box_components;

	};
}