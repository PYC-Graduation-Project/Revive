#pragma once
#include "client/asset/animation/animation_sequence.h"

namespace client_fw
{
	class SkeletalMeshComponent;
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

		void SetAnimationName(const std::string& animation_name); 
		const std::string GetAnimationName() { return m_animation_name; }

		void SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton);

		const float GetCurretPlayTime() const { return m_time_pos; }

		void SetAnimationSpeed(float value) { m_animation_speed = value; }
		const float GetAnimationSpeed() const { return m_animation_speed; }
		void AddNotify(const std::string name, const std::string animation_name, int frame_index, const std::function<void()>& function);
		
		void SetOwner(const SPtr<SkeletalMeshComponent>& owner) { m_owner = owner; }
		SPtr<SkeletalMeshComponent> GetOwner() const { return m_owner.lock(); }

	private:
		std::string m_mesh_path;
		std::string m_animation_name;
		std::string m_ready_animation_name;

		bool m_is_update_animation = false;

		float m_start_time;
		float m_end_time;

		float m_time_pos = 0.0f;

		float m_animation_speed = 1.0f;

		int m_prev_time_index = 0;

		UINT m_instance_index;
		SPtr<AnimationSequence> m_anim_seq = nullptr;
		SPtr<AnimationSequence> m_ready_anim_seq = nullptr;
		WPtr<SkeletalMeshComponent> m_owner;
		std::vector<SPtr<Skeleton>> m_cahce_skeleton;
		std::vector<Mat4> m_bone_offset;
		std::vector<Mat4> m_bone_transform_data;
		
		//Notify가 저장되는 곳
		//원래는 AnimNotify라는 클래스를 따로만들어서 
		//그곳에서 사용되는 애니메이션을 모아서
		//해당 클래스를 플레이어에 멤버로 넣고, 그 객체를 통해서
		//Notify를 등록 & 관리하는게 맞지만 
		//시간관계상 animation controller에 기능을 넣었다.
		std::map<std::string, NotifyData> m_notify_map;
	};
}