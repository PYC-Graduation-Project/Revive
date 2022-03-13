#pragma once
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	class StaticMesh;

	class StaticMeshComponent : public MeshComponent
	{
	public:
		StaticMeshComponent(const std::string& name = "static mesh component");
		virtual ~StaticMeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	public:
		SPtr<StaticMesh> GetStaticMesh() const;
		virtual bool SetMesh(const std::string& file_path);

	protected:
		SPtr<StaticMeshComponent> SharedFromThis();
	};

}

