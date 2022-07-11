#include <include/client_core.h>
#include <client/object/component/render/texture_billboard_component.h>
#include "object/effect/core/effect_actor.h"

namespace revive
{
	EffectActor::EffectActor(eMobilityState mobility,
		const std::string& path, const Vec2& size, bool fix_up)
		:Actor(mobility,"Effect"),m_path(path),m_size(size),m_fix_up(fix_up)
	{
		m_billboard_component = CreateSPtr<TextureBillboardComponent>();
	}
	bool EffectActor::Initialize()
	{
		m_billboard_component->SetTexture(m_path);
		m_billboard_component->SetSize(m_size);
		m_billboard_component->SetFixUpVector(m_fix_up);
		return AttachComponent(m_billboard_component);
	}
}