#pragma once
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	class SkeletalMesh;

	class SkeletalMeshComponent : public MeshComponent
	{
	public:
		SkeletalMeshComponent(const std::string& name = "skeletal mesh component");
		virtual ~SkeletalMeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	public:
		SPtr<SkeletalMesh> GetSkeletalMesh() const;
		virtual bool SetMesh(const std::string& file_path);

	protected:
		virtual UPtr<Collisioner> CreateCollisioner(); //추후 추가
		SPtr<SkeletalMeshComponent> SharedFromThis();
	};

}

