#pragma once
#include "client/asset/animation/animation_sequence.h"

namespace client_fw
{
	class AnimationSeqnece;
	class Skeleton;

	struct BoneData;

	class AnimationController : public std::enable_shared_from_this<AnimationController>
	{
	public:
		AnimationController();
		virtual ~AnimationController() = default;

		bool Initialize();

		const std::vector<Mat4>& GetBoneTransformData() { return m_bone_transform_data; }
		void CopyBoneTransformData();
	public:
		void SetMeshPath(const std::string& mesh_path) { m_mesh_path = mesh_path; }
		const std::string GetAnimationPath(const std::string& animation_name);

		void SetAnimation(const SPtr<Skeleton>& skeleton);
		void AnimToPlay(float delta_time, bool m_looping);

		void SetAnimationName(const std::string& animation_name) { m_animation_name = animation_name; }
		const std::string GetAnimationName() { return m_animation_name; }

		void SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton);

		bool GetIsNeedUpdate() { return m_is_need_update; }
		void SetIsNeedUpdate(bool value) { m_is_need_update = value; }

	private:
		std::string m_mesh_path;
		std::string m_animation_name;

		float m_start_time;
		float m_end_time;

		float m_time_pos = 0.0f;

		float m_animation_speed = 1.0f;

		int m_prev_time_index = 0;

		bool m_is_registered = false;
		bool m_is_need_update = false;

		UINT m_instance_index;
		SPtr<AnimationSequence> m_anim_seq = nullptr;
		std::vector<SPtr<Skeleton>> m_cahce_skeleton;
		std::vector<Mat4> m_bone_offset;
		std::vector<Mat4> m_bone_transform_data;
	};
}