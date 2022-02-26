#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/asset/bone/skeleton.h"

namespace client_fw
{
	void AnimationSequence::Shutdown()
	{
	}
	void AnimationSequence::AnimToPlay(float delta_time, bool m_looping)
	{
		float time_pos = 0.0f;
		if (m_looping)
		{
			time_pos += delta_time;
			if (time_pos < m_start_time) time_pos = m_start_time;
			if (time_pos >= m_end_time) time_pos = m_start_time;
			m_time_pos = time_pos;//애니메이션 콜백함수 사용시 쓸수도잇음,저장
		}
		m_anim_track->TrackToPlay(delta_time, time_pos);
	}
	void AnimationSequence::SetDefaultTime(float s_time, float e_time)
	{
		m_anim_track->SetDefaultTime(s_time, e_time);
		m_start_time = s_time;
		m_end_time = e_time;
	}
	void AnimationTrack::InitialIze(int b_count,float weight )
	{
		m_animated_bone_count = b_count;
		m_wieght = weight;

		m_animated_skeleton.resize(b_count);
		m_anim_curves.resize(b_count);
	}
	void AnimationTrack::TrackToPlay(float delta_time, float time_pos)
	{
		for (int i = 0; i < m_animated_bone_count; ++i)
		{
			m_animated_skeleton.at(i)->SetToParent(GetSRT(i, time_pos,m_wieght));
		}
	}

	const Mat4& AnimationTrack::GetSRT(int bone_index, float time_pos, float weight)
	{
		auto temp_bone = m_animated_skeleton.at(bone_index);
		Vec3 scale = temp_bone->m_scale;
		Vec3 rotate = temp_bone->m_rotation;
		Vec3 trans = temp_bone->m_translation;

		auto& temp_curve = m_anim_curves.at(bone_index);
		if (temp_curve.at(0)) trans.x = temp_curve.at(0)->GetValueByLerp(time_pos);
		if (temp_curve.at(1)) trans.y = temp_curve.at(1)->GetValueByLerp(time_pos);
		if (temp_curve.at(2)) trans.z = temp_curve.at(2)->GetValueByLerp(time_pos);
		if (temp_curve.at(3)) rotate.x = temp_curve.at(3)->GetValueByLerp(time_pos);
		if (temp_curve.at(4)) rotate.y = temp_curve.at(4)->GetValueByLerp(time_pos);
		if (temp_curve.at(5)) rotate.z = temp_curve.at(5)->GetValueByLerp(time_pos);
		if (temp_curve.at(6)) scale.x = temp_curve.at(6)->GetValueByLerp(time_pos);
		if (temp_curve.at(7)) scale.y = temp_curve.at(7)->GetValueByLerp(time_pos);
		if (temp_curve.at(8)) scale.z = temp_curve.at(8)->GetValueByLerp(time_pos);

		Mat4 S = mat4::CreateScale(scale.x * weight, scale.y * weight, scale.z * weight);
		Mat4 R = mat4::CreateRotationX(rotate.x * weight) * mat4::CreateRotationY(rotate.y * weight) * mat4::CreateRotationZ(rotate.z * weight);
		Mat4 T = mat4::CreateTranslation(trans.x * weight, trans.y * weight, trans.z * weight);

		Mat4 transform = S * R * T;

		return transform;
	}
	
	SPtr<AnimationCurve> LoadKeyValue(FILE* file)
	{

		SPtr<AnimationCurve> anim_curve = CreateSPtr<AnimationCurve>();

		int temp_int;

		fread(&temp_int, sizeof(int), 1, file); //key_frame_count
		std::vector<KeyFrame> key_frames(temp_int);

		
		for (auto& key_frame : key_frames)
			fread(&(key_frame.key_time), sizeof(float), 1, file); //여기가 문제였다 같이 읽으면안됨 time 다읽고 value를 읽어야지
		for (auto& key_frame : key_frames)
			fread(&(key_frame.key_value), sizeof(float), 1, file); 

		
		anim_curve->m_key_frames = key_frames;

		return anim_curve;
	}
	float AnimationCurve::GetValueByLerp(float time_pos)
	{
		for (int i = 0; i < (m_key_frames.size() - 1); ++i)
		{
			if ((m_key_frames.at(i).key_time <= time_pos) && (time_pos < m_key_frames.at(i + 1).key_time))
			{
				float t = (time_pos - m_key_frames.at(i).key_time) / (m_key_frames.at(i + 1).key_time - m_key_frames.at(i).key_time);
				return(m_key_frames.at(i).key_value * (1.0f - t) + m_key_frames.at(i + 1).key_value);
			}
		}
		return (m_key_frames.at(m_key_frames.size() - 1).key_value);
	}
}