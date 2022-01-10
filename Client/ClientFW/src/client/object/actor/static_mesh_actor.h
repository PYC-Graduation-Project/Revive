#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class StaticMeshComponent;

	class StaticMeshActor : public Actor
	{
	public:
		StaticMeshActor(eMobilityState mobility, const std::string& static_mesh_path);
		virtual ~StaticMeshActor() = default;

		virtual void Initialize() override;

	protected:
		std::string m_mesh_path;
		WPtr<StaticMeshComponent> m_static_mesh_component;
	};
}

