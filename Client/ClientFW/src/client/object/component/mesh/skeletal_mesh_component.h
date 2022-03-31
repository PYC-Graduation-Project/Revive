#pragma once
#include "client/object/animation/animation_controller.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/asset/bone/skeleton.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	class AnimationSequence;
	class AnimationController;
	class SkeletalMesh;

	class SkeletalMeshComponent : public MeshComponent
	{
	public:
		SkeletalMeshComponent(const std::string& name = "skeletal mesh component",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kSkeletalMesh));
		virtual ~SkeletalMeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	private:
		SPtr<AnimationController> m_animation_controller;

		std::string m_animation_name;
		bool m_looping = true;
		bool m_is_playing = true;

	public:
		SPtr<SkeletalMesh> GetSkeletalMesh() const;
		virtual bool SetMesh(const std::string& file_path);

		const std::vector<Mat4>& GetBoneTransformData() { return m_animation_controller->GetBoneTransformData(); }

		//SPtr<AnimationController>& GetAnimationController() { return m_animation_controller; }

		void SetIsPlaying(const bool is_playing) { m_is_playing = is_playing; }

		void SetAnimation(const std::string& animation_name);
	protected:
		SPtr<SkeletalMeshComponent> SharedFromThis();
	};

}

