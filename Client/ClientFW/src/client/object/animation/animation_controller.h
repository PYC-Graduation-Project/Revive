#pragma once
#include "client/asset/animation/animation_sequence.h"

namespace client_fw
{
	class AnimationSeqnece;
	class Skeleton;

	struct BoneData;
	struct RSSkletalData;

	class AnimationController :public std::enable_shared_from_this<AnimationController>
	{
	public:
		AnimationController();
		virtual ~AnimationController() = default;

		bool Initialize();

		void CopyAnimationData();
		const RSSkletalData& GetBoneTransformData() { return m_bone_trans_data; }

	public:
		void SetAnimation(const std::string& animation_path, const SPtr<Skeleton>& skeleton);
		void AnimToPlay(float delta_time, bool m_looping);

		void SetAnimationName(const std::string& animation_name) { m_anim_seq->anim_name = animation_name; }
		const std::string GetAnimationName() { return m_anim_seq->anim_name; }

		void SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton);

		bool GetIsNeedUpdate() { return m_is_need_update; }
		void SetIsNeedUpdate(bool value) { m_is_need_update = value; }
		
		bool GetIsRegistered() { return m_is_registered; }
		void SetIsRegistered(bool value) { m_is_registered = value; }

	private:
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
		RSSkletalData m_bone_trans_data;
	};
}