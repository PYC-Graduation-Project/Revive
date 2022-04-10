#pragma once
#include "client/asset/animation/animation_sequence.h"

namespace client_fw
{
	class AnimationSeqnece;
	class Skeleton;

	struct BoneData;
	struct NotifyData
	{
		int frame_index;
		std::string animation_name;
		std::function<void()> notify_function;
	};

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

		const float GetCurretPlayTime() const { return m_time_pos; }

		void SetAnimationSpeed(float value) { m_animation_speed = value; }

		void AddNotify(const std::string name, const std::string animation_name, int frame_index, const std::function<void()>& function);

	private:
		std::string m_mesh_path;
		std::string m_animation_name;

		float m_start_time;
		float m_end_time;

		float m_time_pos = 0.0f;

		float m_animation_speed = 1.0f;

		int m_prev_time_index = 0;

		UINT m_instance_index;
		SPtr<AnimationSequence> m_anim_seq = nullptr;
		std::vector<SPtr<Skeleton>> m_cahce_skeleton;
		std::vector<Mat4> m_bone_offset;
		std::vector<Mat4> m_bone_transform_data;
		
		//Notify�� ����Ǵ� ��
		//������ AnimNotify��� Ŭ������ ���θ��� 
		//�װ����� ���Ǵ� �ִϸ��̼��� ��Ƽ�
		//�ش� Ŭ������ �÷��̾ ����� �ְ�, �� ��ü�� ���ؼ�
		//Notify�� ��� & �����ϴ°� ������ 
		//�ð������ animation controller�� ����� �־���.
		std::map<std::string, NotifyData> m_notify_map;
	};
}