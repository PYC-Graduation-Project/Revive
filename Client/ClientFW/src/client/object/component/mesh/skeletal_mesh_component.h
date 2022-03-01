#pragma once
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	class AnimationSequence;
	class SkeletalMesh;

	class SkeletalMeshComponent : public MeshComponent
	{
	public:
		SkeletalMeshComponent(const std::string& name = "skeletal mesh component");
		virtual ~SkeletalMeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	private:
		SPtr<AnimationSequence> m_anim_seq = nullptr;
		bool m_looping = true;
	public:
		SPtr<SkeletalMesh> GetSkeletalMesh() const;
		virtual bool SetMesh(const std::string& file_path);

		void SetAnimation(const std::string& anim_name);
	protected:
		virtual UPtr<Collisioner> CreateCollisioner(); //추후 추가
		SPtr<SkeletalMeshComponent> SharedFromThis();
	};

}

