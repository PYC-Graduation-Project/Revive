#pragma once
#include "client/asset/animation/animation_sequence.h"

namespace client_fw
{
	class AnimationSeqnece;
	class Skeleton;

	struct BoneData;
	struct AnimationData;

	class AnimationController :public std::enable_shared_from_this<AnimationController>
	{
	public:
		AnimationController();
		virtual ~AnimationController() = default;

		bool Initialize();

		void CopyAnimationData();
		const AnimationData& GetBoneTransformData() { return m_bone_trans_data; }

	public:
		void SetAnimation(const std::string& animation_path, const SPtr<Skeleton>& skeleton);
		void AnimToPlay(float delta_time, bool m_looping);

		void SetAnimationName(const std::string& animation_name) { m_anim_seq->anim_name = animation_name; }
		const std::string GetAnimationName() { return m_anim_seq->anim_name; }

		void SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton);

		bool GetIsNeedUpdate() { return m_is_need_update; }
		void SetIsNeedUpdate(bool value) { m_is_need_update = value; }

	private:
		bool m_is_need_update = false;
		UINT m_instance_index;
		SPtr<AnimationSequence> m_anim_seq = nullptr;
		std::vector<SPtr<Skeleton>> m_cahce_skeleton;
		std::vector<Mat4> m_bone_offset;
		AnimationData m_bone_trans_data;
	};
}