#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/asset/bone/skeleton.h"

namespace client_fw
{
	void AnimationSequence::Shutdown()
	{
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

		m_cache_skeleton.resize(b_count);
		m_anim_curves.resize(b_count);

		

	}
	SPtr<AnimationCurve> LoadKeyValue(FILE* file)
	{

		SPtr<AnimationCurve> anim_curve = CreateSPtr<AnimationCurve>();

		int temp_int;
		KeyFrame temp_key;

		fread(&temp_int, sizeof(int), 1, file); //key_frame_count
		std::vector<KeyFrame> key_frames(temp_int);

		float value;
		for (int k = 0; k < temp_int; ++k)
		{
			fread(&temp_key.key_time, sizeof(float), 1, file);
			fread(&temp_key.key_value, sizeof(float), 1, file);
			key_frames.emplace_back(std::move(temp_key));
		}
		anim_curve->m_key_frames = key_frames;

		return anim_curve;
	}
}