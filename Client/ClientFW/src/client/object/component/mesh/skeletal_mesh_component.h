#pragma once
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/asset/bone/skeleton.h"

namespace client_fw
{
	class AnimationSequence;
	class AnimationController;
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
		SPtr<AnimationController> m_animation_controller;

		std::string m_animation_name;
		bool m_looping = true;
		bool m_is_playing = true;

		bool RegisterAnimationController();
		void UnregisterAnimationController();
	public:

		SPtr<SkeletalMesh> GetSkeletalMesh() const;
		virtual bool SetMesh(const std::string& file_path);

		SPtr<AnimationController>& GetAnimationController() { return m_animation_controller; }

		void SetIsPlaying(const bool is_playing) { m_is_playing = is_playing; }

		void SetAnimation(const std::string& animation_path, const std::string& animation_name);
	protected:
		virtual UPtr<Collisioner> CreateCollisioner(); //추후 추가
		SPtr<SkeletalMeshComponent> SharedFromThis();
	};

}

