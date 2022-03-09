#pragma once
#include "client/asset/core/asset.h"

namespace client_fw 
{
	class Skeleton;

	struct AnimationData
	{
		std::array<Mat4,32> animation_matrix;
	};

	struct KeyFrame
	{
		float key_time;
		float key_value;
	};

	class AnimationCurve
	{
	public:
		AnimationCurve() = default;
		virtual ~AnimationCurve() = default;
	private:
	public:
		std::vector<KeyFrame> m_key_frames;
		float GetValueByLerp(int prev_time_index, float time_pos);

	};
	class AnimationTrack
	{
	public:
		AnimationTrack() = default;
		virtual ~AnimationTrack() = default;
	private:
		float m_start_time;
		float m_end_time;

		int m_animated_bone_count;
		float m_wieght;

		int m_prev_time_index = 0;//위치변경

		std::vector<SPtr<Skeleton>> m_animated_skeleton;

		std::vector<std::vector<SPtr<AnimationCurve>>> m_anim_curves;
	public:
		void InitialIze(int b_count, float weight);//멤버 변수, 벡터 초기화

		void SetDefaultTime(float s_time, float e_time) { m_start_time = s_time; m_end_time = e_time; }
		void SetAnimationCurves(const std::vector<std::vector<SPtr<AnimationCurve>>>& curves) { m_anim_curves = curves; }
		void SetCacheSkel(const std::vector<SPtr<Skeleton>>& animated_skeleton) { m_animated_skeleton = animated_skeleton; }
		
		void SearchKeyFrame(float time_pos, const std::vector<KeyFrame>& key_frames);


		void TrackToPlay(float time_pos);
		
		const Mat4& GetSRT(int bone_index,float time_pos, float weight);
		std::vector<SPtr<Skeleton>>& GetAnimatedSkeleton() { return m_animated_skeleton; }

	};

	class AnimationSequence : public Asset
	{
	public:
		AnimationSequence() = default;
		virtual ~AnimationSequence() = default;
		virtual void Shutdown() override;

	private:
		float m_start_time;
		float m_end_time;

		float m_time_pos = 0.0f;//위치변경

		float m_animation_speed = 1.0f;
	
	public:
		std::string anim_name;
		SPtr<AnimationTrack> m_anim_track;

		void AnimToPlay(float delta_time, bool m_looping);
		void SetDefaultTime(float s_time, float e_time);

		const std::vector<SPtr<Skeleton>>& GetAnimatedSkeleton() { return m_anim_track->GetAnimatedSkeleton(); }
		
		void SetAnimationTrack(const SPtr<AnimationTrack>& anim_track) { m_anim_track = anim_track; }

		const float GetAnimationSpeed() { return m_animation_speed; }
		void SetAnimationSpeed(float speed) { m_animation_speed = speed; }

	};

	SPtr<AnimationCurve> LoadKeyValue(FILE* file);

}