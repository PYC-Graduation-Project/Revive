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
		bool m_set_mesh = false;

	public:
		SPtr<SkeletalMesh> GetSkeletalMesh() const;
		virtual bool SetMesh(const std::string& file_path);

		const std::vector<Mat4>& GetBoneTransformData() { return m_animation_controller->GetBoneTransformData(); }

		const Vec3 GetSocketWorldPosition(const std::string& socket_name);
		const Mat4 GetSocketWorldMatrix(const std::string& socket_name);
		const Quaternion GetSocketWorldRotation(const std::string& socket_name);

		const bool IsLooping() { return m_looping; }
		void SetLooping(const bool looping) { m_looping = looping; }

		const bool IsPlaying() { return m_is_playing; }
		void SetIsPlaying(const bool is_playing) { m_is_playing = is_playing; }

		const std::string& GetAnimationName() const { return m_animation_name; }
		void SetAnimation(const std::string& animation_name, const bool looping = true);

		const float GetAnimationSpeed() const { return m_animation_controller->GetAnimationSpeed(); }
		void SetAnimationSpeed(float speed) { m_animation_controller->SetAnimationSpeed(speed); }

		void AddNotify(const std::string name, const std::string animation_name, int frame_index, const std::function<void()>& function) { m_animation_controller->AddNotify(name,  animation_name, frame_index, function); }
	
	protected:
		SPtr<SkeletalMeshComponent> SharedFromThis();
	};

}

